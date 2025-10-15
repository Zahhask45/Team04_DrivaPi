FROM python:3.11-slim
WORKDIR /workspace
RUN pip install --no-cache-dir git+https://gitlab.com/CodethinkLabs/trustable/trustable.git doorstop pyyaml
COPY . /workspace
ENTRYPOINT ["/usr/local/bin/trudag"]
