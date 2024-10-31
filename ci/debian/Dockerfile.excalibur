# syntax=docker/dockerfile:1-labs
FROM devuan/migrated:excalibur-slim

COPY . /calamares

# Update packages
RUN apt update && apt upgrade -y

# Install debhelper
RUN yes | apt install -y debhelper cryptsetup pkg-kde-tools pkexec os-prober rsync || :

# Create .orig tarball
RUN tar -cJf calamares_3.3.10.orig.tar.xz calamares

WORKDIR "/calamares"

# Install dependencies
RUN ./ci/deps-debian11.sh

# Move debian folder to top
RUN mv ./ci/debian .

RUN dpkg-buildpackage
