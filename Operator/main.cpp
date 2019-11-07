//
//  main.cpp
//  Operator
//
//  Created by Victor Garcia Cortes on 03/11/2019.
//  Copyright © 2019 VictorGarciaCortes. All rights reserved.
//

#include <iostream>

typedef uint8_t  Rgb332;
typedef uint16_t Rgb565;
typedef uint32_t Rgba32;

using namespace std;

typedef union
{
    enum Components
    {
        R, G, B, A
    };
    
    uint32_t value;
    uint8_t Components[4];
}RGBA32;


#pragma mark Sin Optimizar
inline Rgb565 convertNotOptimize(Rgba32 color32)
{
    /*
        // DESEMPAQUETADO
        
        // Arquitectura Little Endian
        // RAM ------------------RGBA------------------
        // CPU ----
        //     ABGR
        
        //   11111111 11111111 11111111 10101011   0xFFFFFFAB
        //   00000000 00000000 00000000 11111111   0x000000FF
        // & -------------------------------------------------
        //   00000000 00000000 00000000 10101011   0x000000AB
        
        // Implementación no rápida debido a que hay 5 operaciones
        
    */
    Rgb565 color16;
    
    unsigned r8 =  color32        & 0xff;
    unsigned g8 = (color32 >> 5 ) & 0xff;
    unsigned b8 = (color32 >> 11) & 0xff;
    
    unsigned r5 = r8 >> 3;
    unsigned g6 = g8 >> 2;
    unsigned b5 = b8 >> 3;
    
    color16 = (r5 << 11) | (g6 << 5) | b5;
    
    return color16;
}

#pragma mark Optimizacion 1
inline Rgb565 convertOptimized_1(Rgba32 color32)
{
    Rgb565 color16;
    
    // Implementación rápida ya que accedemos a los componentes de la variable mediante un puntero
    uint8_t * p32 = (uint8_t *)&color32;

    uint16_t r8 = p32[0];
    uint16_t g8 = p32[1];
    uint16_t b8 = p32[2];
      
    //REDUCCIÓN

    uint16_t r5 = r8 >> 3;
    uint16_t g6 = g8 >> 2;
    uint16_t b5 = b8 >> 3;

    // REEMPAQUETAR

    // Esta arquitectura no afecta en little endian debido a que parte de los componentes están partidos en los bites
    // Por esto mismo little endian no puede dar la vuelta a las variables por que no puede
    color16 = uint16_t((r5 << 11) | (g6 << 5) | b5);
    
    return color16;
}

#pragma mark Optimizacion 2
inline Rgb565 convertOptimize_2(Rgba32 color32)
{
    Rgb565 color16;
    
    //Optimización para evitarnos el puntero
    uint16_t r8 = ((uint8_t*) &color32)[0];
    uint16_t g8 = ((uint8_t*) &color32)[1];
    uint16_t b8 = ((uint8_t*) &color32)[2];
    
    uint16_t r5 = r8 >> 3;
    uint16_t g6 = g8 >> 2;
    uint16_t b5 = b8 >> 3;
     
    // Esta arquitectura no afecta en little endian debido a que parte de los componentes están partidos en los bites
    // Por esto mismo little endian no puede dar la vuelta a las variables por que no puede
    color16 = uint16_t((r5 << 11) | (g6 << 5) | b5);
    
    return color16;
}


#pragma mark Optimizacion 3
inline Rgb565 convertOptimize_3(Rgba32 color32)
{
    // Ahorro de alguna operación
    
    uint16_t r8 = ((uint8_t*) &color32)[0];
    uint16_t g8 = ((uint8_t*) &color32)[1];
    uint16_t b8 = ((uint8_t*) &color32)[2];
    
    return uint16_t
    (
        (r8 >> 3 << 11) |
        (g8 >> 2 << 5)  |
        (b8 >> 3     )
    );
}

void Test()
{
    RGBA32 c;
    c.Components[RGBA32::R] = 0xAA;
    c.Components[RGBA32::G] = 0xBB;
    c.Components[RGBA32::R] = 0xCC;
    c.Components[RGBA32::R] = 0xDD;
    
    Rgb565 c16 = convertOptimize_3(c.value);
}

int main(int argc, const char * argv[])
{
    return 0;
}


