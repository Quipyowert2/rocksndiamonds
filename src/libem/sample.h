#ifndef SAMPLE_H
#define SAMPLE_H

enum {
	SAMPLE_blank = 0, /* player walks on blank */
	SAMPLE_roll, /* player pushes stone/bomb/nut */
	SAMPLE_stone, /* stone hits ground */
	SAMPLE_nut, /* nut hits ground */
	SAMPLE_crack, /* stone hits nut */
	SAMPLE_bug, /* bug moves */
	SAMPLE_tank, /* tank moves */
	SAMPLE_android, /* android places something */
	SAMPLE_spring, /* spring hits ground/wall/bumper, stone hits spring */
	SAMPLE_slurp, /* spring kills alien */
	SAMPLE_eater, /* eater sits/eats diamond */
	SAMPLE_alien, /* alien moves */
	SAMPLE_collect, /* player collects diamond/emerald/dynamite/key/lenses/magnify */
	SAMPLE_diamond, /* diamond/emerald hits ground */
	SAMPLE_squash, /* stone squashes diamond, stone/emerald/diamond thru wonderwall */
	SAMPLE_drip, /* drip hits ground */
	SAMPLE_push, /* player pushes spring/balloon/android */
	SAMPLE_dirt, /* player walks on dirt */
	SAMPLE_acid, /* acid splashes */
	SAMPLE_ball, /* ball places something */
	SAMPLE_grow, /* growing wall grows */
	SAMPLE_wonder, /* wonderwall moves */
	SAMPLE_door, /* player goes thru door */
	SAMPLE_exit, /* player goes in exit */
	SAMPLE_dynamite, /* player places dynamite */
	SAMPLE_tick, /* dynamite ticks */
	SAMPLE_press, /* player presses wheel/wind/switch */
	SAMPLE_wheel, /* wheel moves */
	SAMPLE_boom, /* explosion */
	SAMPLE_time, /* time runs out */
	SAMPLE_die, /* player dies */
	SAMPLE_MAX
};

extern char play[SAMPLE_MAX];
extern int sound_pipe[2];
extern short *sound_data[SAMPLE_MAX];
extern long sound_length[SAMPLE_MAX];

#define MIXER_MAX 4 /* maximum number of samples we can play at once */

enum { AUDIO_ULAW = 0, AUDIO_U8 };

#endif