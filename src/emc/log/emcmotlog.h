#ifndef EMCMOTLOG_H
#define EMCMOTLOG_H

/*
  Data logging declarations

  Modification history:

  13-Mar-2000 WPS added unused attribute to emcmotlog_h to avoid 'defined but not used' compiler warning.
  3-Nov-1999  FMP added EMCMOT_LOG_POS_VOLTAGE
  7-Jun-1999  FMP added 'mag' to vel and accel logging
  21-May-1999  FMP added EMCMOT_LOG_TYPE_TRAJ_POS for Cartesian position
  logged at trajectory rate, and VEL and ACCEL differences also
  7-Feb-1999  FMP added EMCMOT_LOG_TYPE_ALL_FERROR for all axes'
  following error
  20-Aug-1998  FMP added EMCMOT_LOG_TYPE_AXIS_VEL; added type to each
  log item; got rid of different EMCMOT_LOG_MAX for different plats;
  made EMCMOT_LOG_MAX 20000, since 100000 resulted in 4 MB shared memory
  23-Feb-1998  FMP added output pos
  6-Feb-1998  FMP added logging type defines
  22-Dec-1997  FMP created
  */

#include "posemath.h"           /* PmCartesian */

/* ident tag */
#ifndef __GNUC__
#ifndef __attribute__
#define __attribute__(x)
#endif
#endif

static char __attribute__((unused)) emcmotlog_h[] = "$Id$";

/* max log points allowed */
/* Note: this will strongly affect the requirements for shared memory */
#define EMCMOT_LOG_MAX 10000

/* types of logged data */
enum EMCMOT_LOG_TYPE_ENUM {
  EMCMOT_LOG_TYPE_AXIS_POS = 1, /* single axis cmd/actual pos */
  EMCMOT_LOG_TYPE_ALL_INPOS = 2, /* all axes actual input pos */
  EMCMOT_LOG_TYPE_ALL_OUTPOS = 3, /* all axes commanded output pos */
  EMCMOT_LOG_TYPE_CMD = 4,      /* command type and num */
  EMCMOT_LOG_TYPE_AXIS_VEL = 5, /* single axis cmd/actual vel */
  EMCMOT_LOG_TYPE_ALL_FERROR = 6, /* all axes following error */
  EMCMOT_LOG_TYPE_TRAJ_POS = 7, /* Cartesian position at traj rate */
  EMCMOT_LOG_TYPE_TRAJ_VEL = 8, /* Cartesian vel diffs at traj rate */
  EMCMOT_LOG_TYPE_TRAJ_ACC = 9, /* Cartesian accel diffs at traj rate */
  EMCMOT_LOG_TYPE_POS_VOLTAGE = 10 /* all axes measured pos and output V */
};


enum EMCLOG_TRIGGER_TYPE {
  EMCLOG_MANUAL_TRIGGER,
  EMCLOG_DELTA_TRIGGER,
  EMCLOG_OVER_TRIGGER,
  EMCLOG_UNDER_TRIGGER
};

enum EMCLOG_TRIGGER_VAR {
  EMCLOG_TRIGGER_ON_FERROR,
  EMCLOG_TRIGGER_ON_VOLT,
  EMCLOG_TRIGGER_ON_POS,
  EMCLOG_TRIGGER_ON_VEL
};


/* various loggable structs */

/* logs commanded and actual position, single axis, per cycle */
typedef struct
{
  double time;
  double output;
  double input;
} EMCMOT_LOG_AXIS_POS_STRUCT;

/* logs actual input position, all axes, per cycle */
#define EMCMOT_LOG_NUM_AXES 3   /* how many are logged */
typedef struct
{
  double time;
  double input[EMCMOT_LOG_NUM_AXES];
} EMCMOT_LOG_ALL_INPOS_STRUCT;

/* logs commanded output position, all axes, per cycle */
typedef struct
{
  double time;
  double output[EMCMOT_LOG_NUM_AXES];
} EMCMOT_LOG_ALL_OUTPOS_STRUCT;

/* logs commands, per new command */
typedef struct
{
  double time;
  int command;
  int commandNum;
} EMCMOT_LOG_CMD_STRUCT;

/* logs axis cmd and actual vel */
typedef struct
{
  double time;
  double cmdVel;
  double actVel;
} EMCMOT_LOG_AXIS_VEL_STRUCT;

/* logs all axes' following error */
typedef struct
{
  double time;
  double ferror[EMCMOT_LOG_NUM_AXES];
} EMCMOT_LOG_ALL_FERROR_STRUCT;

/* logs Cartesian position at trajectory rate */
typedef struct
{
  double time;
  PmCartesian pos;              /* calculated Cartesian position */
} EMCMOT_LOG_TRAJ_POS_STRUCT;

/* logs Cartesian velocity diffs at trajectory rate */
typedef struct
{
  double time;
  PmCartesian vel;              /* differenced Cartesian velocity */
  double mag;
} EMCMOT_LOG_TRAJ_VEL_STRUCT;

/* logs Cartesian acceleration diffs at trajectory rate */
typedef struct
{
  double time;
  PmCartesian acc;              /* differenced Cartesian acceleration */
  double mag;
} EMCMOT_LOG_TRAJ_ACC_STRUCT;

/* logs measured position and resulting output voltage */
typedef struct
{
  double time;
  double pos;
  double voltage;
} EMCMOT_LOG_POS_VOLTAGE_STRUCT;

/* full EMCMOT_LOG_STRUCT union */
typedef struct
{
  int type;
  union {
    EMCMOT_LOG_AXIS_POS_STRUCT axisPos;
    EMCMOT_LOG_ALL_INPOS_STRUCT allInpos;
    EMCMOT_LOG_ALL_OUTPOS_STRUCT allOutpos;
    EMCMOT_LOG_CMD_STRUCT cmd;
    EMCMOT_LOG_AXIS_VEL_STRUCT axisVel;
    EMCMOT_LOG_ALL_FERROR_STRUCT allFerror;
    EMCMOT_LOG_TRAJ_POS_STRUCT trajPos;
    EMCMOT_LOG_TRAJ_VEL_STRUCT trajVel;
    EMCMOT_LOG_TRAJ_ACC_STRUCT trajAcc;
    EMCMOT_LOG_POS_VOLTAGE_STRUCT posVoltage;
  } item;
} EMCMOT_LOG_STRUCT;

/* full log, with header and union of types */
typedef struct
{
  int type;                     /* type of data logged, as in enum above */
  int size;                     /* elements of log[] array */
  int start;                    /* index of start */
  int end;                      /* index of end */
  int howmany;                  /* how many in log */
  EMCMOT_LOG_STRUCT log[EMCMOT_LOG_MAX];
} EMCMOT_LOG;

extern int emcmotLogInit(EMCMOT_LOG *log, int type, int size);
extern int emcmotLogAdd(EMCMOT_LOG *log, EMCMOT_LOG_STRUCT val);
extern int emcmotLogGet(EMCMOT_LOG *log, EMCMOT_LOG_STRUCT *val);

#endif /* EMCMOTLOG_H */









