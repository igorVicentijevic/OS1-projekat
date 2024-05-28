//
// Created by os on 5/25/24.
//

#ifndef CONSOLE_H
#define CONSOLE_H

#include "../h/BoundedBuffer.hpp"
#include "../Test/buffer.hpp"

class Console {

    //static BoundedBuffer<char> *getCbuffer;
    //static BoundedBuffer<char> *putCbuffer;
    static Buffer * getCbuffer;
    static Buffer * putCbuffer;
    enum BitMaskConsoleStatus
    {
        STATUS_CAN_GET_C= (1 << 0),
        STATUS_CAN_PUT_C = (1 << 5),
    };
    static char temp;

public:
    static bool buffersAreEmpty();
    static void initialize();
    static void handlerOutput();
    static void handlerInput();
    static void putC(char c);
    static char getC();


};



#endif //CONSOLE_H
