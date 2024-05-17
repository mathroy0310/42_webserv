# Problemes à regler

- Gerer les leaks, pour eviter les invalid read les aucune ressource alloue est liberer de la memoire, ducoup apres un siege le site devient tres lent
- Post des fichiers avec le CGI ne fonctionne pas.
- Setup multiple servers with different hostnames (use something like: curl --resolve example.com:80:127.0.0.1 http://example.com/). 
