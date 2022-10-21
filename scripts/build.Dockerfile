FROM jurajn/merlin_mockup:1.0

# installing conan
RUN apt-get update \
  && apt-get install -y git libpcap-dev\
  && apt-get install -y libc6-dev\
  && apt-get install -y zip\
  && apt-get install -y libxml2-dev\
