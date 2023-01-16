# linux image with golang, python and lua
FROM ubuntu:latest

RUN apt-get update

# install golang
RUN apt-get install -y golang golang-golang-x-tools clang-format

# install python
RUN apt-get install -y python3 python3-pip

# install lua
RUN apt-get install -y lua5.2

# install utilities
RUN apt-get install -y git nano

# add to nanorc line numbers
RUN echo "set linenumbers" >> /etc/nanorc

# add Rust
RUN sudo apt install rustc

RUN mkdir app
WORKDIR /app

COPY ./requirements.txt .
RUN pip install -r requirements.txt
RUN rm requirements.txt

CMD ["/bin/bash"]
