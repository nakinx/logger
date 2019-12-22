FROM gcc:latest

COPY . /usr/src/logger
WORKDIR /usr/src/logger

RUN make install

CMD ["./logger_test"]

LABEL Name=logger Version=1.0
