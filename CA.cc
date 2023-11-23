#include "led-matrix.h"

#include <unistd.h>
#include <stdint.h>
#include <math.h>
#include <stdio.h>
#include <signal.h>
#include "Rule110.h"

using rgb_matrix::RGBMatrix;
using rgb_matrix::Canvas;
using rgb_matrix::RuntimeOptions;

volatile bool interrupt_received = false;
static void InterruptHandler(int signo) {
  interrupt_received = true;
}


static void Rule30(Canvas *canvas){

  canvas->Fill(0, 0, 0);
  // Initialize a 64-bit unsigned integer 'state' with the 32nd bit set to '1' and others set to '0'
  uint64_t state = 1u << 31;
  int numRow = 0;

  // Iterate over 32 rows
  for (int i = 0; i < 32; i++) {
    if (interrupt_received)
        return;

    // Iterate over each bit position in 'state', from most significant to least significant
    for (int j = 64; j--;) {
        if (interrupt_received)
            return;
      // Output '1' if the current bit in 'state' is set, otherwise output '-'
      /* state >> j & 1 ? canvas->SetPixel(j, i + numRow,
                     255, 255, 255) : canvas->SetPixel(j, i + numRow,
                     0, 0, 0); */

        bool cur = (state >> j) & 1;
        if (cur == true) {
            canvas->SetPixel(j, i, 255,255,255);
            usleep(1 * 1000);
        } else {
            canvas->SetPixel(j,i,0,0,0);
            usleep(1 * 1000);
        }
        
    }

    // Move to the next line after printing a row
    numRow++;

    // Update the 'state' using the Rule 30 logic: (left, current, right) -> left XOR (current OR right)
    state = (state >> 1) ^ (state | state << 1) & 0xFFFFFFFFFFFFFFFF;
  }

}

void Sierpinski(Canvas *canvas) 
{ 
    canvas->Fill(0, 0, 0);
    for (int y = 0; y <= 32 - 1; y++) { 
  
        // printing space till 
        // the value of y 
        for (int i = 0; i < y; i++) {
            usleep(1 * 1000); 
            canvas->SetPixel(i,y,0,0,0); 
        } 
  
        // printing '*' 
        for (int x = 0; x + y < 32; x++) { 
  
        // printing '*' at the appropriate position 
        // is done by the and value of x and y 
        // wherever value is 0 we have printed '*' 
        if(x & y){
            canvas->SetPixel(x,y,0,0,0);
            usleep(1 * 1000);
            //canvas->SetPixel(x+1,y,0,0,0);
            usleep(1 * 1000);
        }
            
            //cout<<" "<<" "; 
        else {
            canvas->SetPixel(x,y,255,255,255);
            usleep(1 * 1000);
            //canvas->SetPixel(x+1,y,0,0,0);
            usleep(1 * 1000);
        }
            //cout<<"* "; 
        } 
  
        //cout<<endl; 
    } 
} 


static void DrawOnCanvas(Canvas *canvas) {
  /*
   * Let's create a simple animation. We use the canvas to draw
   * pixels. We wait between each step to have a slower animation.
   */
  canvas->Fill(0, 0, 255);

  int center_x = canvas->width() / 2;
  int center_y = canvas->height() / 2;
  float radius_max = canvas->width() / 2;
  float angle_step = 1.0 / 360;
  for (float a = 0, r = 0; r < radius_max; a += angle_step, r += angle_step) {
    if (interrupt_received)
      return;
    float dot_x = cos(a * 2 * M_PI) * r;
    float dot_y = sin(a * 2 * M_PI) * r;
    canvas->SetPixel(center_x + dot_x, center_y + dot_y,
                     255, 0, 0);
    usleep(1 * 1000);  // wait a little to slow down things.
  }
}

int main(int argc, char *argv[]) {
  RGBMatrix::Options defaults;
  defaults.hardware_mapping = "adafruit-hat";  // or e.g. "adafruit-hat"
  defaults.rows = 32;
  defaults.cols = 64;
  defaults.chain_length = 1;
  defaults.parallel = 1;
  //defaults.show_refresh_rate = true;
  RuntimeOptions options;
  options.gpio_slowdown = 4;
  //defaults.pwm_lsb_nanoseconds = 200;
  Canvas *canvas = RGBMatrix::CreateFromFlags(&argc, &argv, &defaults, &options);
  if (canvas == NULL)
    return 1;

  // It is always good to set up a signal handler to cleanly exit when we
  // receive a CTRL-C for instance. The DrawOnCanvas() routine is looking
  // for that.
  signal(SIGTERM, InterruptHandler);
  signal(SIGINT, InterruptHandler);

  Rule110Automaton automaton(32,64, canvas);
  automaton.run();

  //Rule30(canvas);    // Using the canvas.
  //Sierpinski(canvas);

  // Animation finished. Shut down the RGB matrix.
  canvas->Clear();
  delete canvas;

  return 0;
}