#ifndef __input_
#define __input_

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#define JPMASK_UP 0x01
#define JPMASK_UP_SERIES 0x40
#define JPMASK_DOWN 0x02
#define JPMASK_DOWN_SERIES 0x80

struct rtkey_s {
  char left;
  char right;
  char up;
  char down;
  char b;
  char a;
  char select;
  char start;   
};

/* - normal keymapper - *\
+        left:A          | 
+        right:D         | 
+        up:W            |
+        down:S          |
|        b:J             +
|        a:K             +
|        select:I        +
|        start:L         +
\* - normal keymapper - */ 

#ifdef __cplusplus
}
#endif  /* __cplusplus */
#endif