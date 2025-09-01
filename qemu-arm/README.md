# Create a ARM32v7 QEMU Image
Tools to setup a build environment for the luckfox lyra

- Download kernel
```
git clone https://git.kernel.org/pub/scm/linux/kernel/git/stable/linux.git -b v6.1.99 --depth=1 linux-6.1.99
```
- Copy defconfig
```
cp -v arm32v7_qemu_defconfig linux-6.1.99/arch/arm/configs/
```
- Create filesystem
```
fallocate -l 4G qemu-arm32v7.img; \
sudo mount --mkdir -o loop qemu-arm32v7.img qemu-fs
```
- Switch to docker container
```
docker build --rm -f lyra-ubuntu.dockerfile -t arm:arm-build .&& \
docker run --rm -it -v $PWD:/build -w /build --user $(id -u):$(id -g) arm:arm-build
```
- Build kernel
```
export ARCH=arm && export CROSS_COMPILE=arm-linux-gnueabihf-; \
(cd linux-6.1.99 && make arm32v7_qemu_defconfig); \
(cd linux-6.1.99 && make -j12)
```
- Create base system
```
sudo debootstrap \
        --arch=armhf \
        --keyring=/usr/share/keyrings/ubuntu-archive-keyring.gpg \
        --verbose \
        --foreign \
        jammy \
        ./qemu-fs
```
- Exit docker container
```
exit
```
- Unmount and boot QEMU
```
sudo umount qemu-fs && rm -d qemu-fs && \
qemu-system-aarch64 \
    -M virt,highmem=off \
    -cpu cortex-a15 \
    -smp cores=4 \
    -m 3G \
    -kernel "linux-6.1.99/arch/arm/boot/zImage" \
    -append "console=ttyAMA0 init=/bin/sh rootwait root=/dev/sda rw" \
    -netdev user,id=net0,hostfwd=tcp::4444-:22 \
        -device virtio-net-device,netdev=net0 \
    -drive file="qemu-arm32v7.img",id=hd,if=none,media=disk,format=raw \
        -device virtio-scsi-device \
        -device scsi-hd,drive=hd \
    -serial mon:stdio -nographic
```
- Finish Installation
```
./debootstrap/debootstrap --second-stage
```
- Set login password
```
passwd
```
- Shutdown and reboot
	- Press Ctrl + a, then x
```
qemu-system-aarch64 \
    -M virt,highmem=off \
    -cpu cortex-a15 \
    -smp cores=4 \
    -m 3G \
    -kernel "linux-6.1.99/arch/arm/boot/zImage" \
    -append "console=ttyAMA0 rootwait root=/dev/sda rw" \
    -netdev user,id=net0,hostfwd=tcp::4444-:22 \
        -device virtio-net-device,netdev=net0 \
    -drive file="qemu-arm32v7.img",id=hd,if=none,media=disk,format=raw \
        -device virtio-scsi-device \
        -device scsi-hd,drive=hd \
    -serial mon:stdio -nographic
```
- Setup network
```
cat <<EOF > /etc/systemd/network/00-wired.network
[Match]
Name=eth0
[Network]
DHCP=yes
EOF
```
- Start networkd
```
systemctl enable systemd-networkd.service && \
systemctl start systemd-networkd.service
```
- Add sources
```
cat <<EOF > /etc/apt/sources.list
deb http://ports.ubuntu.com/ubuntu-ports jammy main universe restricted multiverse
deb http://ports.ubuntu.com/ubuntu-ports jammy-updates main universe restricted multiverse
deb http://ports.ubuntu.com/ubuntu-ports jammy-security main universe restricted multiverse
deb http://ports.ubuntu.com/ubuntu-ports jammy-backports main universe restricted multiverse
EOF
```
- Update
```
apt update && apt upgrade -y
```
- Install ssh server and log into the machine using that for better console support
```
apt install openssh-server && \
echo "PermitRootLogin yes" >> /etc/ssh/sshd_config && \
systemctl ssh restart
```
- Login from a seperate terminal (leave qemu running in background)
```
sh root@127.0.0.1 -p 4444
```