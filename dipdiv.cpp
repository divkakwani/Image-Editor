#include<iostream>
#include<fstream>
#include<windows.h>
#include<string.h>
using namespace std;
ifstream ifile;
ofstream ofile("output.bmp",ios::binary);
class ImgHeader{
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
       void readHeader(){
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
	  }
	 void writeHeader(){
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
      	
};
class image:public ImgHeader{
    public:
	char file_name[100];
	int padding;
      int row_size;
      unsigned char* row;
   	int i,j;
	image(){
    		developers();
    		cout<<"\nEnter image file name(a 24 bit bmp file)\t";
      		cin>>file_name;
      		ifile.open(file_name,ios::binary);
      		if(!ifile){  //If File doesnt exist,exit the program
           		 cerr<<"\n\n\tFile Not Found!";
           		 exit(17);
     		 }
      		readHeader();
      		padding=(4-((width*3)%4))%4;
            row_size=width*3+padding;
            row=new unsigned char[row_size];
      		display_info();
      		if(!(format[0]=='B'&& format[1]=='M') || bpp!=24){
           		 cerr<<"\n\n\t\tInvalid Format !!";
           		 exit(3);
     		 }
    	}
    	~image(){
    		delete row;
    	}
    	void grayscale();
     	void negative();
     	void brightness(float intensity);
     	void pixelate(float intensity,int y1,int y2,int x1,int x2);
     	void color_boost(float intensity);
     	void contrast(float intensity);
        void crop(int hcut,int vcut);
        void resize(int n);
    	void display_info(){
              cout<<"\nThe size of the image is :  "<<size/1024.0<<"  Kilobytes(KB)";
              cout<<"\nThe width of the image is :  "<<width<<"  Pixels";
              cout<<"\nThe height of the image is :  "<<height<<"  Pixels\n\n";
        }    
     	void developers(){
              cout<<"\n\n\tDevelopers of the program:\n \n"
              	  <<"\tDivyanshu Kakwani,XII Sci,120017\n"
              	  <<"\tDiptanshu Kakwani,XII Sci,120016\n";
         }
};
void image::grayscale(){
	writeHeader();
	unsigned char avg;
    int i(0),j(0);
    while(i++<height){  
         ifile.read((char*)row,row_size);
         j=0;
         while(j<row_size-padding){
             avg=(row[j]+row[j+1]+row[j+2])/3;
             row[j]=avg;
             row[j+1]=avg;
             row[j+2]=avg;
             j=j+3;
         }
         ofile.write((char*)row,row_size);
    }
}
void image::negative(){
	writeHeader();
	i=0,j=0;
    while(i++<height){  
        ifile.read((char*)row,row_size);
        j=0;
        while(j<row_size-padding){
             row[j]=255-row[j];
             j=j+1;
        }
        ofile.write((char*)row,row_size);
    }
}
void image::brightness(float intensity){
	writeHeader();
	i=0,j=0;
    while(i++<height){  
    	ifile.read((char*)row,row_size);
        j=0;
        while(j<row_size-padding){
        	  if(row[j]+intensity>255)  row[j]=255;
        	  else if(row[j]+intensity<0)row[j]=0;
              else row[j]+=intensity;
              j=j+1;
        }
        ofile.write((char*)row,row_size);
    }
}
void image::pixelate(float intensity,int y1,int y2,int x1,int x2){
   	  writeHeader();
      i=0,j=0;
      unsigned char avg[3]={0,0,0};
      while(i++<height){  
            ifile.read((char*)row,row_size);
            j=x1;
            if(i>y1 &&i<y2){
                  while(j<x2){
                        for(int n=0;n<3*intensity;n++)  { avg[(j+n)%3]+=row[j+n]/intensity;}
                        for(int n=0;n<3*intensity;n++) {   row[j+n]=avg[(j+n)%3];    }
                         j+=3*intensity;
                         avg[0]=avg[1]=avg[2]=0;
                  }
        	}
            
            ofile.write((char*)row,row_size);
      }
}      
void image::color_boost(float intensity){
	  writeHeader();
      i=0,j=0;
      while(i++<height){  
            ifile.read((char*)row,row_size);
            j=0;
            while(j<row_size-padding){
                  if(row[j]+row[j]/(100-intensity)>255)	row[j]=255;
				  else if(row[j]+row[j]/(100-intensity)<0) row[j]=0;
				  else row[j]+=row[j]/(100-intensity);            
                  j++;
        	}
            ofile.write((char*)row,row_size);
      }
}
void image::contrast(float intensity){
	  writeHeader();
      i=0,j=0;
      while(i++<height){  
      		ifile.read((char*)row,row_size);
            j=0;
            while(j<row_size-padding){
                  if(row[j]>=150)
                  {		if((row[j]+(row[j]-150)/intensity)>255)
                             row[j]=255;
                        else  row[j]+=(row[j]-150)/intensity;
                  }
                  if(row[j]<150){
                        if((row[j]-(150-row[j])/intensity)>0)
                              row[j]-=(150-row[j])/intensity;
                        else row[j]=0;
                  }
                  j++;
            }
            ofile.write((char*)row,row_size);
      }
}
void image::crop(int hcut,int vcut){
      width=width-2*hcut;
      height=height-2*vcut;
      int new_padding=(4-((width*3)%4))%4;
      size=(height)*(width)*3+54+new_padding*height;
      int image_size=size-54;
      int new_row_size=width*3+new_padding;
      writeHeader();
      i=0,j=0;
      unsigned char* new_row=new unsigned char[new_row_size];
      ifile.seekg(vcut*row_size,ios::cur);
      while(i++<height){  
            ifile.read((char*)row,row_size);
            j=hcut*3;
            while(j<new_row_size+hcut*3){
                if(j<row_size-padding)
					new_row[j-hcut*3]=row[j];
				else	new_row[j]=0;
                j=j+1;
            } 
      ofile.write((char*)new_row,new_row_size);
      }
      delete new_row;
}
void image::resize(int n){
      width=width/n;
      height=height/n;
      int new_padding=(4-((width*3)%4))%4;
      int size=(height)*(width)*3+54+new_padding*height;
      int image_size=size-54;
      int new_row_size=width*3+new_padding;
      writeHeader();
      i=0,j=0;
      unsigned char* new_row=new unsigned char[new_row_size];
      while(i++<height){  
            ifile.read((char*)row,row_size);
            j=0;
            while(j<new_row_size){
            	if(j<new_row_size-padding)
				  	new_row[j]=row[n*(j- (j%3) )+(j%3)];
				else	new_row[j]=0;
                    j++;
            }
            ofile.write((char*)new_row,width*3+new_padding);
            ifile.seekg((n-1)*row_size,ios::cur);
      }
      delete new_row;
}
int main(){
	  system("TITLE DIPDIV's Image Editor");
	  system("color 3F");
	  cout<<"\t\t\tDIPDIV's Image editor\t\t\n";
	  image img;    // Show developers' info
      int choice;
      // -----  Menu   -------  
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
                        img.grayscale();
                        break;
            case 2:
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
                        cout<<"\n Enter the intensity(>0),the smaller the better\t";
                        cin>>intensity;
                        img.contrast(intensity);
                        break;
            case 7:
                        int y1,y2,x1,x2,power;
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
      cout<<"\n\tSuccees!!\n";
      system("\"\"output.bmp\"\"");
      system("Pause");
      return 0;
}
