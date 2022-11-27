
all:
	docker build -t bench-builder .
	docker run -e TARGET_UID=$$(id -u) -e CARGO_HOME=/src/.cargo_home -v $$(pwd):/src bench-builder bash -c /src/bench.sh
