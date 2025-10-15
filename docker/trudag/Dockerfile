FROM python:3.11-slim
WORKDIR /workspace

# Install git and build dependencies so pip can build packages from VCS
RUN apt-get update \
	&& apt-get install -y --no-install-recommends \
	git \
	build-essential \
	gcc \
	g++ \
	libffi-dev \
	libssl-dev \
	&& rm -rf /var/lib/apt/lists/*

RUN python -m pip install --upgrade pip setuptools wheel

# Install Trustable and dependencies
COPY docker/trudag/install_trustable.sh /tmp/install_trustable.sh
RUN chmod +x /tmp/install_trustable.sh && /tmp/install_trustable.sh
RUN pip install --no-cache-dir doorstop pyyaml

COPY . /workspace
ENTRYPOINT ["/usr/local/bin/trudag"]
