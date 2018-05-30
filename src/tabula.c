#include "tabula.h"
#include "notes.h"

// (60000 / (note prescaler * standard tempo ))
//  mathmatically, this should be 60000/(4*150) = 100, but it plays too slowly
#define tempo 40

static const unsigned char tabula_track1[][2] = {
    {0, 2}, {A5, 2}, {0, 2}, {A5, 2}, {0, 2}, {Gs5, 2}, {0, 2}, {Gs5, 2},
    {0, 2}, {A5, 2}, {0, 2}, {A5, 2}, {Gs5, 2}, {A5, 2}, {B5, 2}, {Cs6, 2},
    {0, 2}, {A5, 2}, {0, 2}, {A5, 2}, {0, 2}, {Gs5, 2}, {0, 2}, {Gs5, 2},
    {0, 2}, {A5, 2}, {0, 2}, {A5, 2}, {Gs5, 2}, {A5, 2}, {B5, 2}, {Cs5, 1},  {E5, 1}, //33
    
    {Fs5, 12}, {0, 2}, {Cs5, 1}, {E5, 1},
    {Fs5, 4}, {A5, 4}, {Gs5, 2}, {E5, 4}, {Cs5, 1}, {E5, 1},
    {Fs5, 16},
    {0, 12}, {Cs4, 2}, {E4, 2}, // 14
    
    {Fs4, 6}, {A4, 2}, {Gs4, 2}, {A4, 1}, {Gs4, 1}, {Fs4, 2}, {E4, 2},
    {E4, 1}, {Fs4, 1}, {Cs4, 4}, {B3, 2}, {Cs4, 4}, {B3, 2}, {A3, 2},
    {B3, 6}, {A3, 2}, {Gs3, 2}, {B3, 2}, {E4, 2}, {Fs4, 2},
    {Cs4, 12}, {Cs4, 2}, {E4, 2}, // 23
    
    {Fs4, 6}, {A4, 2}, {Gs4, 2}, {A4, 1}, {Gs4, 1}, {Fs4, 2}, {E4, 2},
    {E4, 1}, {Fs4, 1}, {Cs4, 4}, {B3, 2}, {Cs4, 4}, {B3, 2}, {A3, 2},
    {B3, 6}, {A3, 2}, {Gs3, 4}, {E3, 4},
    {Fs3, 2}, {Cs3, 2}, {Gs3, 2}, {Cs3, 2}, {A3, 4}, {A3, 2}, {B3, 2}, // 25
    
    {Cs4, 12}, {E4, 2}, {Fs4, 2},
    {Cs4, 12}, {B3, 2}, {A3, 2},
    {B3, 6}, {A3, 2}, {Gs3, 2}, {B3, 2}, {E4, 2}, {Fs4, 2},
    {Cs4, 12}, {Fs4, 2}, {E4, 2}, // 15
    
    {Fs4, 6}, {E4, 2}, {Fs4, 2}, {B3, 2}, {Cs4, 2}, {E4, 2},
    {A3, 2}, {B3, 2}, {Cs4, 2}, {Gs4, 2}, {A4, 4}, {A4, 2}, {B4, 2},
    {A4, 2}, {Gs4, 2}, {Fs4, 2}, {A4, 2}, {Gs4, 2}, {Fs4, 2}, {E4, 2}, {Gs4, 2},
    {Fs4, 2}, {Gs4, 2}, {A4, 2}, {B4, 2}, {Cs5, 4}, {Cs5, 2}, {E5, 2}, // 28
    
    {Cs5, 8}, {E5, 4}, {Cs5, 2}, {E5, 2},
    {Cs5, 4}, {Gs5, 4}, {A5, 4}, {Gs5, 4},
    {Fs5, 2}, {E5, 2}, {Cs5, 2}, {Fs5, 2}, {E5, 2}, {B4, 2}, {Gs4, 2}, {E5, 2},
    {Cs5, 4}, {B4, 4}, {Cs5, 4}, {A4, 2}, {B4, 2}, // 21
    
    {Cs5, 8}, {E5, 4}, {Cs5, 2}, {E5, 2},
    {Cs5, 4}, {Gs5, 4}, {A5, 4}, {B5, 4},
    {A5, 2}, {Gs5, 2}, {Fs5, 2}, {A5, 2}, {Gs5, 2}, {E5, 2}, {B4, 2}, {E5, 2},
    {Cs5, 2}, {E5, 2}, {B4, 2}, {Cs5, 2}, {A4, 2}, {A4, 2}, {Gs4, 4}, // 23
    
    {Cs5, 8}, {E5, 4}, {Cs5, 2}, {E5, 2},
    {Cs5, 4}, {Gs5, 4}, {A5, 4}, {Gs5, 4},
    {Fs5, 2}, {E5, 2}, {Cs5, 2}, {Fs5, 2}, {E5, 2}, {B4, 2}, {Gs4, 2}, {E5, 2},
    {Cs5, 4}, {B4, 4}, {Cs5, 4}, {A4, 2}, {B4, 2}, // 21
    
    {Cs5, 8}, {E5, 4}, {Cs5, 2}, {E5, 2},
    {Cs5, 4}, {Gs5, 4}, {A5, 4}, {B5, 4},
    {A5, 2}, {Gs5, 2}, {Fs5, 2}, {A5, 2}, {Gs5, 2}, {E5, 2}, {B4, 2}, {E5, 2},
    {Fs5,16} // 17
};

static const unsigned char tabula_track2[][2] = {
    {0, 2}, {Cs4, 2}, {0, 2}, {Cs4, 2}, {0, 2}, {B3, 2}, {0, 2}, {B3, 2},
    {0, 2}, {A3, 2}, {0, 2}, {A3, 2}, {0, 2}, {B3, 2}, {0, 2}, {B3, 2},
    {0, 2}, {Cs4, 2}, {0, 2}, {Cs4, 2}, {0, 2}, {B3, 2}, {0, 2}, {B3, 2},
    {0, 2}, {A3, 2}, {0, 2}, {A3, 2}, {0, 2}, {B3, 2}, {0, 2}, {B3, 2}, // 32
    
    {0, 2}, {A5, 2}, {0, 2}, {A5, 2}, {0, 2}, {Gs5, 2}, {0, 2}, {Gs5, 2},
    {0, 2}, {A5, 2}, {0, 2}, {A5, 2}, {Gs5, 2}, {A5, 2}, {B5, 2}, {Cs6, 2},
    {0, 2}, {A5, 2}, {0, 2}, {A5, 2}, {0, 2}, {Gs5, 2}, {0, 2}, {Gs5, 2},
    {0, 2}, {A5, 2}, {0, 2}, {A5, 2}, {Gs5, 2}, {A5, 2}, {B5, 2}, {Cs6, 2}, //32
    
    {0, 16},
    {0, 16},
    {0, 16},
    {Gs3, 2}, {Cs4, 2}, {A3, 2}, {B3, 2}, {Cs4, 2}, {Cs4, 1}, {B3, 1}, {A3, 2}, {Gs3, 2}, // 12
    
    {0, 16},
    {0, 16},
    {0, 16},
    {0, 16}, //4
    
    {0, 4}, {Cs5, 2}, {E5, 2}, {Fs5, 8},
    {0, 4}, {Cs5, 2}, {E5, 2}, {Fs5, 8},
    {0, 4}, {Cs5, 2}, {E5, 2}, {Fs5, 2}, {Cs5, 2}, {Gs5, 2}, {A5, 2},
    {A5, 8}, {Gs5, 8}, // 17
    
    {Fs5, 6}, {E5, 2}, {Fs5, 2}, {B4, 2}, {Cs5, 2}, {E5, 2},
    {A4, 2}, {B4, 2}, {Cs5, 2}, {Gs5, 2}, {A5, 4}, {A5, 2}, {B5, 2},
    {A5, 2}, {Gs5, 2}, {Fs5, 2}, {A5, 2}, {Gs5, 2}, {Fs5, 2}, {E5, 2}, {Gs5, 2},
    {Fs5, 2}, {Gs5, 2}, {A5, 2}, {B5, 2}, {Cs6, 4}, {0, 4}, // 27
    
    {Cs3, 2}, {Fs3, 2}, {E3, 2}, {Fs3,2}, {Cs3, 2}, {Fs3, 2}, {E3, 2}, {Fs3, 2},
    {Cs3, 2}, {Fs3, 2}, {E3, 2}, {Fs3,2}, {Cs3, 2}, {Fs3, 2}, {E3, 2}, {Fs3, 2},
    {B2, 2}, {A3, 2}, {Gs3, 2}, {A3,2}, {0, 2}, {B3, 2}, {E3, 2}, {Gs3, 2},
    {Fs3, 2}, {A3, 2}, {Gs3, 2}, {E3, 2}, {Cs3, 2}, {A2, 2}, {Cs2, 2}, {E2, 2}, //32
    
    {Cs3, 2}, {Fs3, 2}, {E3, 2}, {Fs3,2}, {Cs3, 2}, {Fs3, 2}, {E3, 2}, {Fs3, 2},
    {Cs3, 2}, {Fs3, 2}, {E3, 2}, {Fs3,2}, {Cs3, 2}, {Fs3, 2}, {E3, 2}, {Fs3, 2},
    {B2, 2}, {A3, 2}, {Gs3, 2}, {A3,2}, {E3, 2}, {B3, 2}, {A3, 2}, {B3, 2},
    {D3, 2}, {Fs3, 2}, {E3, 2}, {Gs3, 2}, {A2, 2}, {Fs3, 2}, {E3, 2}, {B2, 2}, //32
    
    {Cs3, 2}, {Fs3, 2}, {E3, 2}, {Fs3,2}, {Cs3, 2}, {Fs3, 2}, {E3, 2}, {Fs3, 2},
    {Cs3, 2}, {Fs3, 2}, {E3, 2}, {Fs3,2}, {Cs3, 2}, {Fs3, 2}, {E3, 2}, {Fs3, 2},
    {B2, 2}, {A3, 2}, {Gs3, 2}, {A3,2}, {0, 2}, {B3, 2}, {E3, 2}, {Gs3, 2},
    {Fs3, 2}, {A3, 2}, {Gs3, 2}, {E3, 2}, {Cs3, 2}, {A2, 2}, {Cs2, 2}, {E2, 2}, //32
    
    {Cs3, 2}, {Fs3, 2}, {E3, 2}, {Fs3,2}, {Cs3, 2}, {Fs3, 2}, {E3, 2}, {Fs3, 2},
    {Cs3, 2}, {Fs3, 2}, {E3, 2}, {Fs3,2}, {Cs3, 2}, {Fs3, 2}, {E3, 2}, {Fs3, 2},
    {B2, 2}, {A3, 2}, {Gs3, 2}, {A3,2}, {E3, 2}, {B3, 2}, {A3, 2}, {B3, 2},
    {Cs3, 2}, {Fs3, 2}, {E3, 2}, {Fs3, 2}, {Fs2, 8} // 29
};

static const unsigned char tabula_track3[][2] = {
    {Fs2, 2}, {Fs3, 2}, {Cs3, 2}, {Fs3, 2}, {E2, 2}, {E3, 2}, {B2, 2}, {E3, 2},
    {D2, 2}, {D3, 2}, {A2, 2}, {D3, 2}, {E2, 2}, {E3, 2}, {B2, 2}, {Cs3, 2},
    {Fs2, 2}, {Fs3, 2}, {Cs3, 2}, {Fs3, 2}, {E2, 2}, {E3, 2}, {B2, 2}, {E3, 2},
    {D2, 2}, {D3, 2}, {A2, 2}, {D3, 2}, {E2, 2}, {E3, 2}, {B2, 2}, {Cs3, 2}, // 32
    
    {Fs2, 2}, {Fs3, 2}, {Cs3, 2}, {Fs3, 2}, {E2, 2}, {E3, 2}, {B2, 2}, {E3, 2},
    {D2, 2}, {D3, 2}, {A2, 2}, {D3, 2}, {E2, 2}, {E3, 2}, {B2, 2}, {Cs3, 2},
    {Fs2, 2}, {Fs3, 2}, {Cs3, 2}, {Fs3, 2}, {E2, 2}, {E3, 2}, {B2, 2}, {E3, 2},
    {D2, 2}, {D3, 2}, {A2, 2}, {D3, 2}, {E2, 2}, {E3, 2}, {B2, 2}, {E3, 2}, // 32
    
    {Fs2, 2}, {Cs3, 2}, {Fs2, 2}, {Cs3, 2}, {Fs2, 2}, {Cs3, 2}, {Fs2, 2}, {Cs3, 2},
    {D2, 2}, {A2, 2}, {D2, 2}, {A2, 2}, {D2, 2}, {A2, 2}, {D2, 2}, {A2, 2},
    {E2, 2}, {B2, 2}, {E2, 2}, {B2, 2}, {E2, 2}, {B2, 2}, {E2, 2}, {B2, 2},
    {Fs2, 2}, {Cs2, 2}, {Fs2, 2}, {Cs2, 2}, {F2, 2}, {Cs2, 2}, {F2, 2}, {Cs2, 2}, // 32
    
    {Fs2, 2}, {Cs3, 2}, {Fs2, 2}, {Cs3, 2}, {Fs2, 2}, {Cs3, 2}, {Fs2, 2}, {Cs3, 2},
    {D2, 2}, {A2, 2}, {D2, 2}, {A2, 2}, {D2, 2}, {A2, 2}, {D2, 2}, {A2, 2},
    {E2, 2}, {B2, 2}, {E2, 2}, {B2, 2}, {E2, 2}, {B2, 2}, {E2, 2}, {B2, 2},
    {Fs2, 2}, {Cs2, 2}, {Fs2, 2}, {Cs2, 2}, {F2, 2}, {Cs2, 2}, {F2, 2}, {Cs2, 2}, // 32
    
    {Fs3, 6}, {Fs2, 2}, {Fs2, 8},
    {Fs3, 6}, {Fs2, 2}, {Fs2, 8},
    {E3, 6}, {E3, 2}, {E3, 8},
    {D3, 8}, {E3, 8}, // 11
    
    {Fs2, 2}, {Cs3, 2}, {Fs2, 2}, {Cs3, 2}, {Fs2, 2}, {Cs3, 2}, {Fs2, 2}, {Cs3, 2},
    {D2, 2}, {A2, 2}, {D2, 2}, {A2, 2}, {D2, 2}, {A2, 2}, {D2, 2}, {A2, 2},
    {E2, 2}, {B2, 2}, {E2, 2}, {B2, 2}, {E2, 2}, {B2, 2}, {E2, 2}, {B2, 2},
    {Fs2, 2}, {Gs2, 2}, {A2, 2}, {B2, 2}, {Cs3, 4}, // 29
    
    {Fs2, 16},
    {A2, 16},
    {A2, 8}, {Fs2, 4}, {Gs2, 4},
    {A2, 8}, {Cs3, 8}, // 7
    
    {Fs2, 16},
    {A2, 16},
    {A2, 8}, {B2, 8},
    {Fs2, 4}, {Gs2, 4}, {A2, 2}, {Fs2, 2}, {E2, 4}, // 9
    
    {Fs2, 16},
    {A2, 16},
    {A2, 8}, {Fs2, 4}, {Gs2, 4},
    {A2, 8}, {Cs3, 8}, // 7
    
    {Fs2, 16},
    {A2, 16},
    {A2, 8}, {B2, 8},
    {Fs2, 16} // 9
};

float get_Tabula_Tone(unsigned char track, unsigned char index)
{ 
    switch(track)
    {
        case 0:
            return get_Frequency(tabula_track1[index][0]);
            break;
        case 1:
            return get_Frequency(tabula_track2[index][0]);
            break;
        case 2:
            return get_Frequency(tabula_track3[index][0]);
            break;
        default:
            return 0;
            break;
    }
}

unsigned long get_Tabula_Duration(unsigned char track, unsigned char index)
{
    switch(track)
    {
        case 0:
            return (unsigned long)tabula_track1[index][1] * tempo;
            break;
        case 1:
            return (unsigned long)tabula_track2[index][1] * tempo;
            break;
        case 2:
            return (unsigned long)tabula_track3[index][1] * tempo;
            break;
        default:
            return 0;
            break;
    }
}

unsigned char get_Tabula_Size(unsigned char track)
{ 
    switch(track)
    {
        case 0:
            return sizeof tabula_track1 / sizeof *tabula_track1;
            break;
        case 1:
            return sizeof tabula_track2 / sizeof *tabula_track2;
            break;
        case 2:
            return sizeof tabula_track3 / sizeof *tabula_track3;
            break;
        default:
            return 0;
            break;
    }
}