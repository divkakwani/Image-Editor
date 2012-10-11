#include<iostream>
#include<fstream>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

using namespace std;
ifstream ifile;
ofstream ofile("output.bmp",ios::binary);

class image{
      public:
      char format[2];
      int size;
      short int reserved1;
      short int reserved2;
      int start_pos;
      int header_size;
      int width;
      int height;
      short int color_planes;
      short int bpp;
      int comp_method;
      int image_size;
      int hres;
      int vres;
      int colors_in_palette;
      int imp_colors;
      int padding;
      int row_size;
      void read1(){
            ifile.read((char*)&format,2);
            ifile.read((char*)&size,4);
            ifile.read((char*)&reserved1,2);
            ifile.read((char*)&reserved2,2);
            ifile.read((char*)&start_pos,4);
            ifile.read((char*)&header_size,4);
            ifile.read((char*)&width,4);
            ifile.read((char*)&height,4);
            ifile.read((char*)&color_planes,2);
            ifile.read((char*)&bpp,2);
            ifile.read((char*)&comp_method,4);
            ifile.read((char*)&image_size,4);
            ifile.read((char*)&hres,4);
            ifile.read((char*)&vres,4);
            ifile.read((char*)&colors_in_palette,4);
            ifile.read((char*)&imp_colors,4);
            padding=(4-((width*3)%4))%4;
            row_size=width*3+padding;
      }
      
      void write1(){
            ofile.write((char*)&format,2);
            ofile.write((char*)&size,4);
            ofile.write((char*)&reserved1,2);
            ofile.write((char*)&reserved2,2);
            ofile.write((char*)&start_pos,4);
            ofile.write((char*)&header_size,4);
            ofile.write((char*)&width,4);
            ofile.write((char*)&height,4);
            ofile.write((char*)&color_planes,2);
            ofile.write((char*)&bpp,2);
            ofile.write((char*)&comp_method,4);
            ofile.write((char*)&image_size,4);
            ofile.write((char*)&hres,4);
            ofile.write((char*)&vres,4);
            ofile.write((char*)&colors_in_palette,4);
            ofile.write((char*)&imp_colors,4);
      }
      
      void negative()
      {
            int i(0),j(0);
            unsigned char row[50000];
            while(i++<height){  
                  ifile.read((char*)&row,row_size);
                  j=0;
                  while(j<row_size-padding){
                        row[j]=255-row[j];
                        j=j+1;
                  }
                  ofile.write((char*)&row,row_size);
            }
      }
      
            void grayscale()
      {
            unsigned char avg;
            int i(0),j(0);
            unsigned char row[50000];
            while(i++<height){  
                  ifile.read((char*)&row,row_size);
                  j=0;
                  while(j<row_size-padding){
                        avg=(row[j]+row[j+1]+row[j+2])/3;
                        row[j]=avg;
                        row[j+1]=avg;
                        row[j+2]=avg;
                        j=j+3;
                  }
                  ofile.write((char*)&row,row_size);
            }
      }

      void brightness(float intensity)
      {
            int i(0),j(0);
            unsigned char row[50000];
            while(i++<height){  
                  ifile.read((char*)&row,row_size);
                  j=0;
                  while(j<row_size-padding){
                        if(intensity+row[j]>255) row[j]=255;
                        else if(intensity+row[j]<0) row[j]=0;
                        else row[j]=intensity+row[j];
                        j=j+1;
                  }
                  ofile.write((char*)&row,row_size);
            }

      }
      
      void crop(int hcut,int vcut)
      {
            int width1=width-2*hcut;
            int height1=height-2*vcut;
            int padding1=(4-((width1*3)%4))%4;
            int size1=(height1)*(width1)*3+54+padding1*height1;
            int image_size1=size1-54;
            
            
            ofile.write((char*)&format,2);
            ofile.write((char*)&size1,4);
            ofile.write((char*)&reserved1,2);
            ofile.write((char*)&reserved2,2);
            ofile.write((char*)&start_pos,4);
            ofile.write((char*)&header_size,4);
            ofile.write((char*)&width1,4);
            ofile.write((char*)&height1,4);
            ofile.write((char*)&color_planes,2);
            ofile.write((char*)&bpp,2);
            ofile.write((char*)&comp_method,4);
            ofile.write((char*)&image_size1,4);
            ofile.write((char*)&hres,4);
            ofile.write((char*)&vres,4);
            ofile.write((char*)&colors_in_palette,4);
            ofile.write((char*)&imp_colors,4);
            
            int padding=(4-((width*3)%4))%4;
            int row_size=width*3+padding;
            int i(0),j(0);
            unsigned char row[50000];
            unsigned char row1[50000];
            while(i++<height1){  
                  ifile.read((char*)&row,row_size);
                  j=hcut*3;
                  while(j<row_size-padding){
                        row1[j-hcut*3]=row[j];
                        j=j+1;
                  }
                  row1[width1*3]=row1[width*3+1]=row1[width*3+2]=0;
                  ofile.write((char*)&row1,width1*3+padding1);
            }
      }
      
      void resize(int n)
      {
            int width1=width/n;
            int height1=height/n;
            int padding1=(4-((width1*3)%4))%4;
            int size1=(height1)*(width1)*3+54+padding1*height1;
            int image_size1=size1-54;
            
            
            ofile.write((char*)&format,2);
            ofile.write((char*)&size1,4);
            ofile.write((char*)&reserved1,2);
            ofile.write((char*)&reserved2,2);
            ofile.write((char*)&start_pos,4);
            ofile.write((char*)&header_size,4);
            ofile.write((char*)&width1,4);
            ofile.write((char*)&height1,4);
            ofile.write((char*)&color_planes,2);
            ofile.write((char*)&bpp,2);
            ofile.write((char*)&comp_method,4);
            ofile.write((char*)&image_size1,4);
            ofile.write((char*)&hres,4);
            ofile.write((char*)&vres,4);
            ofile.write((char*)&colors_in_palette,4);
            ofile.write((char*)&imp_colors,4);
            
            int i(0),j(0);
            unsigned char row[50000];
            unsigned char row1[50000];
            while(i++<height1){  
                  ifile.read((char*)&row,row_size);
                  j=0;
                  while(j<row_size-padding){
                        row1[j]=row[n*(j- (j%3) )+(j%3)];
                        j++;
                  }
            
                  row1[width1*3]=row[width1*3+1]=row[width1*3+2]=0;
                  ofile.write((char*)&row1,width1*3+padding1);
                  ifile.seekg((n-1)*row_size,ios::cur);
            }
      }
       void pixelate(float intensity,int y1,int y2,int x1,int x2)
      {
            int i(0),j(0);
            unsigned char row[50000];
            unsigned char avg[3]={0,0,0};
            cout<<avg[0];
            while(i++<height){  
                  ifile.read((char*)&row,row_size);
                  j=x1;
            if(i>y1 &&i<y2)
                  while(j<x2){
                        for(int n=0;n<3*intensity;n++)  { avg[(j+n)%3]+=row[j+n]/intensity;}
                        for(int n=0;n<3*intensity;n++) {   row[j+n]=avg[(j+n)%3];    }
                         j+=3*intensity;
                         avg[0]=avg[1]=avg[2]=0;
                  }
            
                  ofile.write((char*)&row,row_size);
            }

      }
      
      void color_boost(float intensity)
      {
            int i(0),j(0);
            unsigned char row[50000];
            while(i++<height){  
                  ifile.read((char*)&row,row_size);
                  j=0;
                  while(j<row_size-padding){
                        if((row[j]+row[j]/(100-intensity))>255)row[j]=255;
                        else row[j]+=row[j]/(100-intensity);                       
                        j++;
                  }
                  ofile.write((char*)&row,row_size);
            }

      }
      void contrast(float intensity)
      {
            int i(0),j(0);
            unsigned char row[50000];
            while(i++<height){  
                  ifile.read((char*)&row,row_size);
                  j=0;
                  while(j<row_size-padding){
                        if(row[j]>=150)
                        {
                              if((row[j]+(row[j]-150)/intensity)<255)
                              row[j]+=(row[j]-150)/intensity;
                              else row[j]=255;
                        }
                        if(row[j]<150){
                              if((row[j]-(150-row[j])/intensity)>0)
                              row[j]-=(150-row[j])/intensity;
                              else row[j]=0;
                        }
                        j++;
                  }
                  ofile.write((char*)&row,row_size);
            }

      }

      void display_info(){
            cout<<"\nThe size of the image is :  "<<size/1024.0<<"  Kilobytes(KB)";
            cout<<"\nThe width of the image is :  "<<width<<"  Pixels";
            cout<<"\nThe height of the image is :  "<<height<<"  Pixels\n\n";
      }      
      
      void developers(){
            cout<<"\n\nThis image editor is jointly developed by Diptanshu and Divyanshu Kakwani\n \n";
      }
            
            
}img;
int main(){
      char file_name[100];
      int choice;
      cout<<"\t\t\tDIPDIV's Image editor\t\t\n";
      img.developers();
      cout<<"\nEnter image name(24 bit bmp file including .bmp)\t";
      cin>>file_name;
      
      ifile.open(file_name,ios::binary);
      if(!ifile){                               //If File doesnt exist,exit the program
            cerr<<"\n\n\tFile Not Found!";
            exit(17);
      }
      
      img.read1();
      img.display_info();
      if(strcmp(img.format,"BM") || img.bpp!=24){
            cerr<<"\n\n\t\tInvalid Format !!";
            exit(3);
      }
      
      cout<<"\n\t Press:\n";
      cout<<"\t 1 To convert image into grayscale\n";
      cout<<"\t 2 To convert into negative\n";
      cout<<"\t 3 To crop the image\n";
      cout<<"\t 4 To increase or decrease the brightness of image\n";
      cout<<"\t 5 To resize the image\n";  
      cout<<"\t 6 To add contrast \n";
      cout<<"\t 7 To pixelate the image\n";
      cout<<"\t 8 To boost the color\n";
      cin>>choice;
      
      switch(choice){
            case 1:  
                        img.write1();
                        img.grayscale();
                        break;
            case 2:
                        img.write1();
                        img.negative();
                        break;
            case 3:
                        int h,v;
                        cout<<"\n Enter the no of pixels you want to remove from left and right each \t";
                        cin>>h;
                        cout<<"\n Enter the no of pixels you want to remove from top and bottom each \t";
                        cin>>v;
                        img.crop(h,v);
                        break;
            case 4:
                        int degree;
                        img.write1();
                        cout<<"\nEnter the intensity of brightness(-255.0 to 255.0)\t";
                        cin>>degree;
                        img.brightness(degree);
                        break;
            case 5:
                        int n;
                        cout<<"\n By what fraction do you want to reduce image size (positive integer)\t";
                        cin>>n;
                        img.resize(n);
                        break;
            case 6:
                        int intensity;
                        img.write1();
                        cout<<"\n Enter the intensity(>0),the smaller the better\t";
                        cin>>intensity;
                        img.contrast(intensity);
                        break;
            case 7:
                        int y1,y2,x1,x2,power;
                        img.write1();
                        cout<<"\nEnter the coordinates of the part you want to pixelate\n";
                        cout<<"\n Enter no of pixels from bottom: \t";
                        cin>>y1;
                        cout<<"\nEnter no of pixels from top: \t";
                        cin>>y2;
                        cout<<"\n Enter no of pixels from left: \t";
                        cin>>x1;
                        cout<<"\nEnter no of pixels from right: \t";
                        cin>>x2;
                        cout<<"\nEnter the intensity of pixelation(integer)(>1 & small)\t";
                        cin>>power;
                        img.pixelate(power,y1,y2,x1,x2);
                        break;
            case 8:
                        int boost_power;
                        img.write1();
                        cout<<"\n Enter the intensity of boost(0 to 100.0 for positive boost and >100.0 for negative boost)\t";
                        cin>>boost_power;
                        img.color_boost(boost_power);
                        break;
            default:
                        cout<<"Invalid Choice";
                        exit(0);
      }
      
      ifile.close(); 
      ofile.close();
      return 0;
}
