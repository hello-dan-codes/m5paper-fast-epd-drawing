#include <M5EPD.h>

M5EPD_Canvas canvas(&M5.EPD);
M5EPD_Canvas canvas1(&M5.EPD);
M5EPD_Canvas canvas2(&M5.EPD);
M5EPD_Canvas canvas3(&M5.EPD);
M5EPD_Canvas canvas4(&M5.EPD);

m5epd_update_mode_t UPDATEMODE = UPDATE_MODE_DU4;
unsigned int prevX;
unsigned int prevY;
unsigned int prevCanvasSel;

unsigned long timeTaken;

void setup()
{
    M5.begin(true, false, true, false, false);
    M5.EPD.SetRotation(90);
    M5.TP.SetRotation(90);
    M5.EPD.Clear(true);

    canvas.createCanvas(540, 60);
    canvas.setTextSize(2);
    canvas.drawString("Time to update: 0ms" , 20, 20);
    canvas.pushCanvas(0,900,UPDATEMODE);

    canvas1.createCanvas(270, 450);
    canvas1.drawFastVLine(270, 0, 450, 2, 255);
    canvas1.drawFastHLine(0, 448, 270, 2, 255);
    canvas1.pushCanvas(0,0,UPDATEMODE);

    canvas2.createCanvas(270, 450);
    canvas2.drawFastHLine(0, 448, 270, 2, 255);
    canvas2.pushCanvas(270,0,UPDATEMODE);

    canvas3.createCanvas(270, 450);
    canvas3.drawFastVLine(270, 0, 450, 2, 255);
    canvas3.drawFastHLine(0, 448, 270, 2, 255);
    canvas3.pushCanvas(0,450,UPDATEMODE);

    canvas4.createCanvas(270, 450);
    canvas4.drawFastHLine(0, 448, 270, 2, 255);
    canvas4.pushCanvas(270,450,UPDATEMODE);
}

void loop()
{
    if(M5.TP.avaliable() && ! M5.TP.isFingerUp()) {
        M5.TP.update();
        uint16_t x1 = M5.TP.readFingerX(0); // (0) for 1 finger. (1) for 2 fingers
        uint16_t y1 = M5.TP.readFingerY(0); // (0) for 1 finger. (1) for 2 fingers

        unsigned long previousMillis = millis();
        unsigned int canvasSel = 0;

        if (x1 > 270 && y1 < 450) {
            canvasSel = 1;
            x1 = x1 - 270;
        } else if (x1 < 270 && y1 > 450) {
            canvasSel = 2;
            y1 = y1 - 450;
        } else if (x1 > 270 && y1 > 450) {
            canvasSel = 3;
            x1 = x1 - 270;
            y1 = y1 - 450;
        }

        // Reset line origin on canvas change
        if (prevX == 0 || (prevCanvasSel != canvasSel)) {
            prevX = x1;
            prevY = y1;
        }

        if (canvasSel == 0) {
            canvas1.drawLine(prevX,prevY,x1,y1,10,255);
            canvas1.pushCanvas(0,0,UPDATEMODE);
        } else if (canvasSel == 1) {
            canvas2.drawLine(prevX,prevY,x1,y1,10,255);
            canvas2.pushCanvas(270,0,UPDATEMODE);
        } else if (canvasSel == 2) {
            canvas3.drawLine(prevX,prevY,x1,y1,10,255);
            canvas3.pushCanvas(0,450,UPDATEMODE);
        } else if (canvasSel == 3) {
            canvas4.drawLine(prevX,prevY,x1,y1,10,255);
            canvas4.pushCanvas(270,450,UPDATEMODE);
        }

        prevX = x1;
        prevY = y1;
        prevCanvasSel = canvasSel;

        unsigned long currentMillis = millis();

        if (timeTaken == 0) {
            timeTaken = currentMillis - previousMillis;
        }
    } else {
        if (timeTaken > 0) {
            delay(100);
            canvas.drawString("Time to update: "  + String(timeTaken) + "ms", 20, 20);
            canvas.pushCanvas(0, 900,UPDATEMODE);
            timeTaken = 0;
            prevX = 0;
            prevY = 0;
        }
    }
}
