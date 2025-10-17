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

# Allow build-time pins for diagnosability and reproducible images
ARG PIN_DOORSTOP=
ARG PIN_PYYAML=pyyaml
ARG TRUSTABLE_REF=

# Install Trustable and dependencies
COPY docker/trudag/install_trustable.sh /tmp/install_trustable.sh
RUN chmod +x /tmp/install_trustable.sh && TRUSTABLE_REF=${TRUSTABLE_REF:-} /tmp/install_trustable.sh
# Install a pinned doorstop (if provided) and pyyaml
RUN if [ -n "${PIN_DOORSTOP}" ]; then pip install --no-cache-dir "doorstop==${PIN_DOORSTOP}"; else pip install --no-cache-dir doorstop; fi
RUN pip install --no-cache-dir ${PIN_PYYAML}

COPY . /workspace
ENTRYPOINT ["/usr/local/bin/trudag"]
