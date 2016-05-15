#include <iostream>
#include <octave/oct.h>
#include <octave/octave.h>
#include <octave/parse.h>
#include <octave/toplev.h>

#include "GL/osmesa.h"
//~ 
//~ //#ifndef GLAPIENTRY
//~ //#define GLAPIENTRY
//~ //#endif
//~ 

static int Width = 400;
static int Height = 400;

static void render_image(void)
{

  glClear(GL_COLOR_BUFFER_BIT);

  // Drawing is done by specifying a sequence of vertices.  The way these
  // vertices are connected (or not connected) depends on the argument to
  // glBegin.  GL_POLYGON constructs a filled polygon.
  glBegin(GL_POLYGON);
    glColor3f(1, 0, 0); glVertex3f(-0.6, -0.75, 0.5);
    glColor3f(0, 1, 0); glVertex3f(0.6, -0.75, 0);
    glColor3f(0, 0, 1); glVertex3f(0, 0.75, 0);
  glEnd();

  // Flush drawing command buffer to make drawing happen as soon as possible.
  glFlush();

}

static void
write_targa(const char *filename, const GLubyte *buffer, int width, int height)
{
   FILE *f = fopen( filename, "w" );
   if (f) {
      int i, x, y;
      const GLubyte *ptr = buffer;
      printf ("osdemo, writing tga file \n");
      fputc (0x00, f);	/* ID Length, 0 => No ID	*/
      fputc (0x00, f);	/* Color Map Type, 0 => No color map included	*/
      fputc (0x02, f);	/* Image Type, 2 => Uncompressed, True-color Image */
      fputc (0x00, f);	/* Next five bytes are about the color map entries */
      fputc (0x00, f);	/* 2 bytes Index, 2 bytes length, 1 byte size */
      fputc (0x00, f);
      fputc (0x00, f);
      fputc (0x00, f);
      fputc (0x00, f);	/* X-origin of Image	*/
      fputc (0x00, f);
      fputc (0x00, f);	/* Y-origin of Image	*/
      fputc (0x00, f);
      fputc (Width & 0xff, f);      /* Image Width	*/
      fputc ((Width>>8) & 0xff, f);
      fputc (Height & 0xff, f);     /* Image Height	*/
      fputc ((Height>>8) & 0xff, f);
      fputc (0x18, f);		/* Pixel Depth, 0x18 => 24 Bits	*/
      fputc (0x20, f);		/* Image Descriptor	*/
      fclose(f);
      f = fopen( filename, "ab" );  /* reopen in binary append mode */
      for (y=height-1; y>=0; y--) {
         for (x=0; x<width; x++) {
            i = (y*width + x) * 4;
            fputc(ptr[i+2], f); /* write blue */
            fputc(ptr[i+1], f); /* write green */
            fputc(ptr[i], f);   /* write red */
         }
      }
   }
}

int
main(int argc, char *argv[])
{
  // hier octave als embedded laufen lassen
  //http://www.gnu.org/software/octave/doc/interpreter/Standalone-Programs.html#Standalone-Programs
  
  // die Datei mit load lesen,
  // dann struct2hdl
  
  string_vector args (2);
  args(0) = "embedded";
  args(1) = "-q";

  octave_main (2, args.c_str_vec (), 1);

  octave_idx_type n = 2;
  octave_value_list in;

  for (octave_idx_type i = 0; i < n; i++)
    in(i) = octave_value (5 * (i + 2));

  octave_value_list out = feval ("gcd", in, 1);

  if (! error_state && out.length () > 0)
    std::cout << "GCD of ["
              << in(0).int_value ()
              << ", "
              << in(1).int_value ()
              << "] is " << out(0).int_value ()
              << std::endl;
  else
    std::cout << "invalid\n";

 
//~ 
  //~ std::cout << "Hello Octave world!\n";
//~ 
  //~ int n = 2;
  //~ Matrix a_matrix = Matrix (n, n);
//~ 
  //~ for (octave_idx_type i = 0; i < n; i++)
    //~ for (octave_idx_type j = 0; j < n; j++)
      //~ a_matrix(i,j) = (i + 1) * 10 + (j + 1);
//~ 
  //~ std::cout << a_matrix;
//~ 
   OSMesaContext ctx;
   void *buffer;
   char *filename = NULL;

   if (argc < 2) {
      fprintf(stderr, "Usage:\n");
      fprintf(stderr, "  osdemo filename [width height]\n");
      return 0;
   }

   filename = argv[1];
   if (argc == 4) {
      Width = atoi(argv[2]);
      Height = atoi(argv[3]);
   }

   /* Create an RGBA-mode context */
#if OSMESA_MAJOR_VERSION * 100 + OSMESA_MINOR_VERSION >= 305
   /* specify Z, stencil, accum sizes */
   ctx = OSMesaCreateContextExt( OSMESA_RGBA, 16, 0, 0, NULL );
#else
   ctx = OSMesaCreateContext( OSMESA_RGBA, NULL );
#endif
   if (!ctx) {
      printf("OSMesaCreateContext failed!\n");
      return 0;
   }

   /* Allocate the image buffer */
   buffer = malloc( Width * Height * 4 * sizeof(GLubyte) );
   if (!buffer) {
      printf("Alloc image buffer failed!\n");
      return 0;
   }

   /* Bind the buffer to the context and make it current */
   if (!OSMesaMakeCurrent( ctx, buffer, GL_UNSIGNED_BYTE, Width, Height )) {
      printf("OSMesaMakeCurrent failed!\n");
      return 0;
   }
     

   {
      int z, s, a;
      glGetIntegerv(GL_DEPTH_BITS, &z);
      glGetIntegerv(GL_STENCIL_BITS, &s);
      glGetIntegerv(GL_ACCUM_RED_BITS, &a);
      printf("Depth=%d Stencil=%d Accum=%d\n", z, s, a);
   }

   render_image();

   if (filename != NULL) {
    write_targa(filename, (const GLubyte *) buffer, Width, Height);
   }
   else {
      printf("Specify a filename if you want to make an image file\n");
   }

   printf("all done\n");

   /* free the image buffer */
   free( buffer );

   /* destroy the context */
   OSMesaDestroyContext( ctx );

  clean_up_and_exit (0);

   return 0;
}
