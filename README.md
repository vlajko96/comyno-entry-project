# comyno-entry-project

How to setup environment:

1. sudo apt install docker
2. sudo apt install docker-compose
3. newgrp docker (if group doesn't already exist)
4. sudo usermod -aG docker $USER
5. sudo chmod 777 /var/run/docker.sock (this is optional, if you still have "couldn't connect to docker daemon" error)
6. make build (this will take 10-15 mins, so bring a coffee ;P)
7. make bash p=<absolute path to project>/project-source
