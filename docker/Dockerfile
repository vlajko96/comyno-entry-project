# Use an official Ubuntu as a parent image
FROM ubuntu:18.04

# Set non-interactive mode for apt-get to avoid prompts
ENV DEBIAN_FRONTEND=non-interactive

# Destination for copying container scripts
ARG COMYNO_ENV_IMAGE_SCRIPTS_PATH

# Host name
ARG COMYNO_ENV_CONTAINER

# Update and install necessary packages
RUN apt-get update && \
    apt-get install -y \
    make \
    git \
    gcc-4.8 \
    g++-4.8 \
    libncurses5-dev \
    libgtest-dev \
    lcov \
    gcovr \
    wget \   
    libjson-c-dev \
    locales \
    postgresql-10 \
    sudo \
    nano \
    sed \
    tar \
    ruby-full \
    libboost-all-dev \
    uuid-dev \
    libpq-dev \
    python \
    python-dev \
    python-pip

# Using bash
ENV SHELL=/bin/bash LANG=en_US.UTF-8
RUN /usr/sbin/locale-gen en_US.UTF-8                           &&   \
    echo 'dash dash/sh boolean false' | debconf-set-selections &&   \
    dpkg-reconfigure dash

# Create defalt user used with default uid and gid
RUN groupadd -g 1000 -o comyno
RUN useradd -u 1000 -g 1000 -d /home/comyno -s /bin/bash -r --password "" comyno
RUN mkdir /home/comyno
RUN chown comyno:comyno /home/comyno

# Add user to sudo group
RUN usermod -aG sudo comyno
RUN echo "%sudo ALL=(ALL) NOPASSWD:ALL" >> /etc/sudoers

# Fix warning "user not in sudoers file"
RUN echo "comyno ALL=(ALL:ALL) ALL" > /etc/sudoers

COPY /scripts $COMYNO_ENV_IMAGE_SCRIPTS_PATH

# Install of the cmake version 3.24.0
ARG CMAKE_VERSION=3.24.0
RUN wget https://github.com/Kitware/CMake/releases/download/v${CMAKE_VERSION}/cmake-${CMAKE_VERSION}-linux-x86_64.sh -O cmake.sh; \
    sh cmake.sh --prefix=/usr/local/ --exclude-subdir; \
    rm -rf cmake.sh;

# Expose the port for Postgresql
EXPOSE 5432

# Run the rest of the commands as the ``postgres`` user created during the installation of PostgreSQL
USER postgres

# Create a PostgreSQL role named ``comyno`` with ``comyno`` as the password and then create a database `comynoDB` owned by the ``comyno`` role.
RUN /etc/init.d/postgresql start && \
    psql --command "CREATE USER comyno WITH SUPERUSER PASSWORD 'comyno';" && \
    createdb -O comyno comynoDB && \
    export PGPASSWORD='comyno' && \
    # Create tables and indexes
    psql -U comyno -d comynoDB -h localhost -f $COMYNO_ENV_IMAGE_SCRIPTS_PATH/init.sql

USER root

# Install of qpid
RUN pip install qpid-python
RUN wget https://dlcdn.apache.org/qpid/cpp/1.39.0/qpid-cpp-1.39.0.tar.gz
RUN tar xvzf qpid-cpp-1.39.0.tar.gz
RUN cd qpid-cpp-1.39.0 && \
    mkdir bld && \
    cd bld && \
    cmake .. && \
    make install && \
    cd / && \
    rm -rf /qpid-cpp-1.39.0.tar.gz && \
    rm -rf /qpid-cpp-1.39.0

# Expose the port for Qpid broker
EXPOSE 5672

# Install GCC-9 and G++-9
RUN apt-get install -y software-properties-common
RUN add-apt-repository ppa:ubuntu-toolchain-r/test
RUN apt-get update && \
    apt-get install -y \
    gcc-9 \
    g++-9

# Set GCC-9 and G++-9 as the default compilers
RUN update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-4.8 50 && \
    update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-4.8 50 && \
    update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-9 100 && \
    update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-9 100

# Build and install libpqxx
RUN git clone https://github.com/jtv/libpqxx.git

RUN cd libpqxx && \
    mkdir bld && \
    cd bld && \
    cmake .. -DSKIP_BUILD_TEST=on -DBUILD_SHARED_LIBS=on && \
    cd .. && \
    cmake --build bld && \
    cmake --install bld && \
    cd / && \
    rm -rf /libpqxx

# Set the environment variable for runtime linker
ENV LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH

# Set environment variables for compiler include paths
ENV CPLUS_INCLUDE_PATH=/usr/local/include:$CPLUS_INCLUDE_PATH

# Adjust PostgreSQL configuration so that remote connections to the database are possible.
RUN echo "host all  all    0.0.0.0/0  md5" >> /etc/postgresql/10/main/pg_hba.conf

# And add ``listen_addresses`` to ``/etc/postgresql/10/main/postgresql.conf``
RUN echo "listen_addresses='*'" >> /etc/postgresql/10/main/postgresql.conf

# Add VOLUME to allow backup of configuration, logs, and databases
VOLUME  ["/etc/postgresql", "/var/log/postgresql", "/var/lib/postgresql"]
