// Copyright (C) 2015 Andreas Weber <andy.weber.aw@gmail.com>
//
// This program is free software; you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free Software
// Foundation; either version 3 of the License, or (at your option) any later
// version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
// details.
//
// You should have received a copy of the GNU General Public License along with
// this program; if not, see <http://www.gnu.org/licenses/>.
//
// This code is based on Brian Pauls' src/osdemos/osdemo.c
// from git://anongit.freedesktop.org/mesa/demos

#include <octave/oct.h>
#include "GL/osmesa.h"
#include <GL/glx.h>

static void
Torus(float innerRadius, float outerRadius, int sides, int rings)
{
  int i, j;
  GLfloat theta, phi, theta1;
  GLfloat cosTheta, sinTheta;
  GLfloat cosTheta1, sinTheta1;
  const GLfloat ringDelta = 2.0 * M_PI / rings;
  const GLfloat sideDelta = 2.0 * M_PI / sides;

  theta = 0.0;
  cosTheta = 1.0;
  sinTheta = 0.0;
  for (i = rings - 1; i >= 0; i--)
  {
    theta1 = theta + ringDelta;
    cosTheta1 = cos(theta1);
    sinTheta1 = sin(theta1);
    glBegin(GL_QUAD_STRIP);
    phi = 0.0;
    for (j = sides; j >= 0; j--)
    {
      GLfloat cosPhi, sinPhi, dist;

      phi += sideDelta;
      cosPhi = cos(phi);
      sinPhi = sin(phi);
      dist = outerRadius + innerRadius * cosPhi;

      glNormal3f(cosTheta1 * cosPhi, -sinTheta1 * cosPhi, sinPhi);
      glVertex3f(cosTheta1 * dist, -sinTheta1 * dist, innerRadius * sinPhi);
      glNormal3f(cosTheta * cosPhi, -sinTheta * cosPhi, sinPhi);
      glVertex3f(cosTheta * dist, -sinTheta * dist,  innerRadius * sinPhi);
    }
    glEnd();
    theta = theta1;
    cosTheta = cosTheta1;
    sinTheta = sinTheta1;
  }
}


static void
render_image(void)
{
  GLfloat light_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
  GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
  GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
  GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };
  GLfloat red_mat[]   = { 1.0, 0.2, 0.2, 1.0 };
  GLfloat green_mat[] = { 0.2, 1.0, 0.2, 1.0 };
  GLfloat blue_mat[]  = { 0.2, 0.2, 1.0, 1.0 };


  glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
  glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);

  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_DEPTH_TEST);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-2.5, 2.5, -2.5, 2.5, -10.0, 10.0);
  glMatrixMode(GL_MODELVIEW);

  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

  glPushMatrix();
  glRotatef(20.0, 1.0, 0.0, 0.0);

  glPushMatrix();
  glTranslatef(-0.75, 0.5, 0.0);
  glRotatef(90.0, 1.0, 0.0, 0.0);
  glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, red_mat );
  Torus(0.275, 0.85, 20, 20);
  glPopMatrix();

  glPushMatrix();
  glTranslatef(-0.75, -0.5, 0.0);
  glRotatef(270.0, 1.0, 0.0, 0.0);
  glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, green_mat );
  Torus(0.275, 0.85, 20, 20);
  glPopMatrix();

  glPushMatrix();
  glTranslatef(0.75, 0.0, -1.0);
  glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, blue_mat );
  Torus(0.275, 0.85, 20, 20);
  glPopMatrix();

  glPopMatrix();

  /* This is very important!!!
   * Make sure buffered commands are finished!!!
   */
  glFinish();
}

DEFUN_DLD(debug_osmesa, args, nargout,
          "-*- texinfo -*-\n\
@deftypefn {Loadable Function}  {@var{img}  =} debug_osmesa ()\n\
return a test RGB image using OpenGL and OSMesa.\n\
@end deftypefn")
{
  octave_value_list retval;

  printf("glXGetProcAddress​ %p\n", glXGetProcAddress((const GLubyte*)"glGetIntegerv"));
  //GLint z, s;
  //glGetIntegerv (GL_DEPTH_BITS, &z);
  //glGetIntegerv (GL_STENCIL_BITS, &s);
  //std::cout << "GL_DEPTH_BITS = " << z << std::endl;
  //std::cout << "GL_STENCIL_BITS = " << s << std::endl;

  GLsizei Width = 640;
  GLsizei Height = 480;

  // Create an RGBA-mode context, specify Z=16, stencil=0, accum=0 sizes
  OSMesaContext ctx = OSMesaCreateContextExt (OSMESA_RGBA, 16, 0, 0, NULL);
  if (! ctx)
  {
    error ("debug_osmesa: OSMesaCreateContext failed!\n");
    return retval;
  }

  // Allocate the image buffer
  OCTAVE_LOCAL_BUFFER (GLubyte, buffer, 4 * Width * Height);

  // Bind the buffer to the context and make it current
  if (! OSMesaMakeCurrent (ctx, buffer, GL_UNSIGNED_BYTE, Width, Height))
  {
    error ("debug_osmesa: OSMesaMakeCurrent failed!\n");
    return retval;
  }

  const char *glVendor = (const char *) glGetString(GL_VENDOR);
  const char *glRenderer = (const char *) glGetString(GL_RENDERER);
  const char *glVersion = (const char *) glGetString(GL_VERSION);
  
  if (glVendor)
    std::cout << "OpenGL vendor string: " << glVendor << std::endl;
  if (glRenderer)
    std::cout << "OpenGL renderer string: " << glRenderer << std::endl;
  if (glVersion)
    std::cout << "version string: " << glVersion << std::endl;

  GLint z=1, s=2, ar=3, ag=4, ab=5;
  glGetIntegerv (GL_DEPTH_BITS, &z);
  glGetIntegerv (GL_STENCIL_BITS, &s);
  glGetIntegerv (GL_ACCUM_RED_BITS, &ar);
  glGetIntegerv (GL_ACCUM_GREEN_BITS, &ag);
  glGetIntegerv (GL_ACCUM_BLUE_BITS, &ab);
  std::cout << "GL_DEPTH_BITS       = " << z << std::endl;
  std::cout << "GL_STENCIL_BITS     = " << s << std::endl;
  std::cout << "GL_ACCUM_RED_BITS   = " << ar << std::endl;
  std::cout << "GL_ACCUM_GREEN_BITS = " << ag << std::endl;
  std::cout << "GL_ACCUM_BLUE_BITS  = " << ab << std::endl;

  render_image();

  dim_vector dv (4, Width, Height);

  // FIXME: We expect that GLubyte is 1 Byte long.
  // Adapt code if this isn't always true
  assert (sizeof (GLubyte) == 1);
  uint8NDArray img (dv);
  unsigned char *p = reinterpret_cast<unsigned char*>(img.fortran_vec ());
  memcpy (p, buffer, (4 * Width * Height));

  Array<octave_idx_type> perm (dim_vector (3, 1));
  perm(0) = 2;
  perm(1) = 1;
  perm(2) = 0;

  Array<idx_vector> idx (dim_vector (3, 1));

  // Flip Y
  idx(0) = idx_vector::make_range (Height - 1, -1, Height);
  idx(1) = idx_vector::colon;

  // Remove alpha channel
  idx(2) = idx_vector (0, 3);
  retval = octave_value (img.permute (perm). index(idx));

  OSMesaDestroyContext (ctx);

  return retval;
}

/*
%!test
%! img = debug_osmesa();
%! imwrite (img, "debug_osmesa.png");
%! assert (size (img), [480 640 3])
%! # count ratio of pixels over 20% threshold as fingerprint
%! cnt = squeeze (sum ( sum (img>50)) / numel (img) * 3)
%! # allow 10% tolerance
%! assert (cnt, [0.11259; 0.10842; 0.12163], -0.1)
*/
