#ifndef ENV_H
#define ENV_H

extern int env_isinit;

extern int env_init();
extern int env_put(char *s);
extern int env_put2(char *s, char *t);
extern int env_unset(char *s);
extern /*@null@*/char *env_get(char *s);
extern char *env_pick();
extern void env_clear();
extern char *env_findeq(char *s);

extern char **environ;

#endif
