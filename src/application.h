#ifndef APPLICATION_H
#define APPLICATION_H

GLOBAL struct
{

} gAppState;

INTERNAL bool   init_application ();
INTERNAL void   quit_application ();
INTERNAL void update_application (float dt);
INTERNAL void render_application (float dt);

#endif /* APPLICATION_H */
