# HLS and RISCV-VP

## HLS steps to reproduce

```shell
cd stratus
make sim_B
make sim_V_BASIC
```

## riscv-vp steps to reproduce

Powershell on the host:

```shell
mkdir C:/Users/user/ee6470
cd C:/Users/user/ee6470
git clone https://gitlab.larc-nthu.net/ee6470_TA/docker-images.git
cd C:/Users/user/ee6470/docker-images
docker build -f EE6470/Dockerfile.x86-64 -t ee6470/ubuntu-ee6470:latest ./EE6470
docker run -h ubuntu --rm --cap-add SYS_ADMIN -it -v "C:\\Users\\user:/home/user" ee6470/ubuntu-ee6470:latest
```

/bin/sh on the host:

```shell
mkdir ~/ee6470
cd ~/ee6470
git clone https://gitlab.larc-nthu.net/ee6470_TA/docker-images.git
cd ~/ee6470/docker-images
docker build -f EE6470/Dockerfile.x86-64 -t ee6470/ubuntu-ee6470:latest ./EE6470
docker run -h ubuntu --rm --cap-add SYS_ADMIN -it -e LOCAL_USER_ID=$(id -u $USER) -v $HOME:/home/user ee6470/ubuntu-ee6470:latest
```

/bin/sh in the container as a superuser:

```shell
USER_ID=${LOCAL_USER_ID:-9001}
groupadd -g ${USER_ID} guser
useradd  -m -p user -s /bin/sh -u $USER_ID -G guser user
echo "user ALL=(ALL) ALL" >>/etc/sudoers
export HOME=/home/user
exec /bin/su - user
```

/bin/sh in the container:

```shell
EE6470=~/ee6470
cd ~/ee6470
git clone https://github.com/agra-uni-bremen/riscv-vp.git
cd ~/ee6470/riscv-vp
git submodule update --init --recursive
cd ~/ee6470
git clone https://gitlab.larc-nthu.net/ee6470_TA/sobel_riscv_vp.git
cp -r ~/ee6470/sobel_riscv_vp/riscv-vp/vp/src/platform/* ~/ee6470/riscv-vp/vp/src/platform
cd ~/ee6470
git clone https://github.com/yeebaan/f.git
cp -r ~/ee6470/f/riscv-vp/vp/src/platform/* ~/ee6470/riscv-vp/vp/src/platform
mkdir ~/ee6470/riscv-vp/vp/build
cd ~/ee6470/riscv-vp/vp/build
cmake ..
```

/bin/sh in the container:

```shell
# clean build
rm -r ~/ee6470/riscv-vp/vp/build
mkdir ~/ee6470/riscv-vp/vp/build
cd ~/ee6470/riscv-vp/vp/build
cmake ..
# after editing vp
cp -r ~/ee6470/sobel_riscv_vp/riscv-vp/vp/src/platform/* ~/ee6470/riscv-vp/vp/src/platform
cp -r ~/ee6470/f/riscv-vp/vp/src/platform/* ~/ee6470/riscv-vp/vp/src/platform
cd ~/ee6470/riscv-vp/vp/build
make
# after editing sw
cp -r ~/ee6470/sobel_riscv_vp/riscv-vp/sw/* ~/ee6470/riscv-vp/sw
cp -r ~/ee6470/f/riscv-vp/sw/* ~/ee6470/riscv-vp/sw
cd ~/ee6470/riscv-vp/sw/basic-sobel
make sim
```

view ~/ee6470/riscv-vp/sw/basic-sobel/lena_std_out.bmp using a web browser

forcibly terminate riscv-vp-acc:

```shell
docker ps
docker exec -it $CONTAINER_ID pkill riscv-vp-acc
```
