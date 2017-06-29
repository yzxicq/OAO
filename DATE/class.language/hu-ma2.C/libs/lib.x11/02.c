#include<ImLib3D/ImLib3D.hpp>
#include<ImLib3D/TestPatterns.hpp>
// Basic creation, acces, file 

int main(int argc,char **argv)
{
  // Declare a floating point image. Images are inialized with 0 size
Image3Df image;
 // create a uniform noise image of size 100x100x100
    IP3D::NoiseUniform(Size3D(100,100,100),image);

    // Change somme arbitrary values
    image(20,10,30)=.5;
    image(10,20,40)=.5;

    // Write the image to a file
    image.WriteToFile("example1.im3D"); 
}


