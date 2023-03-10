# linux image with golang, python and lua
FROM ubuntu:latest

RUN apt-get update

# install utilities
RUN apt-get install -y git nano sudo curl cmake
RUN echo "set linenumbers" >> /etc/nanorc

# install Golang
RUN apt-get install -y golang golang-golang-x-tools clang clang-format

# install Python
RUN apt-get install -y python3 python3-pip

# install Lua
RUN apt-get install -y lua5.2

# install Rust
RUN curl https://sh.rustup.rs -sSf | sh -s -- -y
# update crates.io index
RUN mkdir /tmp/crates_update && cd /tmp/crates_update && /root/.cargo/bin/cargo init && /root/.cargo/bin/cargo add bindgen && rm -rf /tmp/crates_update

RUN mkdir /app
WORKDIR /app

COPY ./requirements.txt .
RUN pip install -r requirements.txt
RUN rm requirements.txt

ENV PATH="/root/.cargo/bin:${PATH}"

CMD ["/bin/bash"]
