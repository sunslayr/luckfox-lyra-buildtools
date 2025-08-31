# luckfox-lyra-buildtools
Tools to setup a build environment for the luckfox lyra


 docker run --privileged --rm tonistiigi/binfmt --install all

sudo mkdir -p /etc/docker/
sudo nano /etc/docker/daemon.json

```
{
  "features": {
    "containerd-snapshotter": true
  }
}
```

```
docker run --rm --privileged multiarch/qemu-user-static --reset -p yes
docker build  --platform linux/arm/v7 --rm -f lyra-arm32v7.dockerfile -t lyra:arm32v7-build .
docker run --platform linux/arm/v7 --rm -it -v $PWD:/build -w /build --user $(id -u):$(id -g) lyra:arm32v7-build
```

```
(cd lyra_sdk/kernel-6.1/ && ./scripts/mkimg --dtb rk3506g-luckfox-lyra-picocalc.dtb)
```

