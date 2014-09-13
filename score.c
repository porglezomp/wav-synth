#include "score.h"

#include <stdlib.h>
#include "note.h"

#define NOTE(HZ, MEASURE, BEAT, DURATION) {HZ, (MEASURE-1)*M+BEAT, DURATION, NULL, NULL},
/* REST is just a placeholder for clarity */
#define REST(DURATION)
#define MEASURE

note score[] = {
  /* Super Mario Bros (1985) Overworld / Main Theme */
  /* 4/4 */
  /* half = 200 */
  /* 60/200 = 0.3 = half note, quarter note = 0.15, measure = 0.6 */
#define Q  0.15
#define H  0.30
#define TH 0.20 /* half-note triplets */
#define W  0.60
#define M  0.60
  MEASURE /* measure 1, 0.0 seconds */
  NOTE( F4s, 1, 0.0, Q ) NOTE ( E5 , 1, 0.0, Q )
  NOTE( F4s, 1, 1*Q, Q ) NOTE ( E5 , 1, 1*Q, Q )
  REST( Q )
  NOTE( F4s, 1, 3*Q, Q ) NOTE ( E5 , 1, 3*Q, Q )
  MEASURE /* measure 2, 0.6 seconds*/
  REST( Q )
  NOTE( F4s, 2, 1*Q, Q ) NOTE ( C5 , 2, 1*Q, Q )
  NOTE( F4s, 2, 2*Q, Q ) NOTE ( E5 , 2, 2*Q, Q )
  REST( Q )
  MEASURE /* measure 3, 1.2 seconds */
  NOTE( G4 , 3, 0, Q ) NOTE ( B4 , 2, 0, Q ) NOTE( G5 , 2, 0, Q )
  REST( Q+H )
  MEASURE /* measure 4, 1.8 seconds */
  NOTE( G4 , 4, 0, Q )
  REST( Q+H )
  MEASURE /* measure 5, 2.4 seconds */
  NOTE( E4 , 5, 0.0, Q ) NOTE( C5 , 5, 0.0, Q )
  REST( Q + Q )
  NOTE( C4 , 5, 3*Q, Q ) NOTE( G4 , 5, 3*Q, Q )
  MEASURE /* measure 6, 3.0 seconds */
  REST( H )
  NOTE( G3 , 6, 2*Q, Q ) NOTE( E4 , 6, 2*Q, Q )
  REST( Q )
  MEASURE /* measure 7, 3.6 seconds */
  REST( Q )
  NOTE( C4 , 7, 1*Q, Q ) NOTE( A4 , 7, 1*Q, Q )
  REST( Q )
  NOTE( D4 , 7, 3*Q, Q ) NOTE( B4 , 7, 3*Q, Q )
};

int score_len = sizeof(score)/sizeof(note);
