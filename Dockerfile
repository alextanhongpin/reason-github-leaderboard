FROM nginx:1.13.12

COPY ./build/ /usr/share/nginx/html

# COPY conf /etc/nginx

# VOLUME /usr/share/nginx/html

# VOLUME /etc/nginx
