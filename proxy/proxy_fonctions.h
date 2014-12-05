#ifndef __PROXY_FONCTIONS_H__
#define __PROXY_FONCTIONS_H__

void premier_appel_app_ext(int csock, int sock_cacheujf, char * s_id_post_inf_comp);
void second_appel_app_ext(int csock, char gros_tampon[3310720]);

#endif
