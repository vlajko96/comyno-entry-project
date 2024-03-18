### Parameters
export PROJECT_PATH=$(p)

export PROJECT_PATH_DOCKER=/project

export COMYNO_ENV_IMAGE=comyno_env_image
export COMYNO_ENV_CONTAINER=comyno_env

### Export fixed paths
export COMYNO_ENV_IMAGE_SCRIPTS_PATH=/comyno_env_container/scripts

.DEFAULT_GOAL := help

help:
	@grep -E '^[a-zA-Z_-]+:.*?## .*$$' $(MAKEFILE_LIST) | awk 'BEGIN {FS = ":.*?## "}; {printf "\033[36m%-30s\033[0m %s\n", $$1, $$2}'

build: ## Build image
	docker-compose --file docker/comyno-project.yaml build

start: ## Get into the container console [p=(path to project folder)]
	docker-compose --file docker/comyno-project.yaml  \
		run --rm \
		${COMYNO_ENV_CONTAINER} ${COMYNO_ENV_IMAGE_SCRIPTS_PATH}/enter-bash.sh

attach:
	@$(eval CONTAINER_ID=$(shell docker ps -q -f name=${COMYNO_ENV_CONTAINER}))
	docker exec -it ${CONTAINER_ID} /bin/bash

stop:
	docker-compose --file docker/comyno-project.yaml stop

remove:
	docker-compose --file docker/comyno-project.yaml down

clean: stop remove
