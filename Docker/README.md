## Howto build docker image

```
$ docker login gitlab.ta.philips.com:5432
$ docker build -t gitlab.ta.philips.com:5432/ius/ius/centos-7.4.1708:0.3 .
$ docker push gitlab.ta.philips.com:5432/ius/ius/centos-7.4.1708:0.3
```

## Howto use docker image manually

Open a terminal into your container to use gcc/valgrind.

```
$ docker run --rm -it -v $(pwd):/data -w /data gitlab.ta.philips.com:5432/ius/ius/centos-7.4.1708:0.3 bash
```
