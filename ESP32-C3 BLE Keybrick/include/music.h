#ifndef __MUSIC_H__
#define __MUSIC_H__

/*
  Doodle Frequencies
        Eb      F       G       Ab      Bb      C       D
  H1    622     698     785     832     932     1047    1176
  H2    1245    1398    1569    1661    1865    2093    2349

  160bpm -> 93.75ms per beat
*/

/*
  Ging Gang Goolie Frequencies
        C       D       E       G       A    
  H1    523     587     659     785     880
  H2    1047    1176    1319    1569    1760

  100bpm -> 150ms per beat
*/

// Doodle Notes
#define Eb 622
#undef F
#define F 698
#define G 785
#define Ab 832
#define Bb 932
#define C 1047
#define D 1176

// Ging Gang Goolie Notes
#define Do 523
#define Re 587
#define Mi 659
#define Sol 785
#define La 880

#define BEAT_QUARTER_DOODLE 94
#define BEAT_QUARTER_GGG 150

const int doodle_intro[] = {
    Bb, 0, Eb, 0,    Bb, 0, Eb, 0,    Bb, 0, Eb, 0,    Bb, 0, Eb, 0
};

const int doodle_main[] = {
    Bb, 0, Eb, 0,   Eb, 0, F, 0,    G, 0, 0, 0,     F, 0, 0, 0,     Eb, 0, Eb, Eb,     Eb, 0, F, 0,     G, 0, 0, 0,     F, 0, 0, 0, 
    Bb, 0, Eb, 0,   Eb, 0, F, 0,    G, 0, 0, 0,     F, 0, 0, 0,     Eb, 0, Eb, Eb,     Eb, 0, Ab, 0,    G, 0, 0, 0,     F, 0, 0, 0 
};

const int ggg_main[] = {
    Do, 0, Mi, 0,   Sol, Sol, Sol, Sol,    Sol, 0, La, 0,     Sol, 0, Mi, 0,     Do, 0, Mi, 0,     Re, 0, 0, 0,     Do, 0, Re, 0,     Mi, 0, 0, 0
};

#endif
