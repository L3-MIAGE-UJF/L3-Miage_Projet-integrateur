#ifndef __PROXY_FONCTIONS_H__
#define __PROXY_FONCTIONS_H__

int recup_param_port(int argc, char * argv[]);

void premier_appel_app_ext(int csock, int sock_cacheujf, char * s_id_post_inf_comp);
void second_appel_app_ext(int csock, char * s_id_post_inf_comp);

#endif
