# Traceability checker

This repository includes a simple, dependency-free traceability checker at `scripts/traceability_check.py`.

What it does
- Scans `reqs/` YAML files and extracts common link patterns (`links:`, `ref:`, `satisfied-by:`).
- Produces `artifacts/traceability-matrix.csv` with columns: `req_id,target_id,link_type,source_file`.

Quick run
```bash
python3 scripts/traceability_check.py reqs --output artifacts/traceability-matrix.csv
```

CI
- Add a pipeline step that runs the checker and fails on `--fail-on-unlinked` if you want to prevent isolated requirements.

S3 uploads (optional)
---------------------

The workflow optionally uploads the generated CSV and Markdown report to S3 when repository secrets or an OIDC role are configured. To enable this feature, add one of the following to repository secrets:

Option A (recommended, OIDC role):
- `TRACEABILITY_ROLE_ARN` — IAM Role ARN that the Actions job can assume via GitHub OIDC.

Option B (fallback, long-lived keys):
- `AWS_ACCESS_KEY_ID` — IAM user access key id
- `AWS_SECRET_ACCESS_KEY` — IAM user secret access key
- `TRACEABILITY_S3_BUCKET` — the target S3 bucket name

Optional secrets/values:

- `AWS_REGION` — AWS region (defaults to `us-east-1`)
- `TRACEABILITY_S3_PREFIX` — key prefix inside the bucket (defaults to `traceability`)
- `TRACEABILITY_S3_SSE` — set to `AES256` to enable SSE-S3
- `TRACEABILITY_KMS_KEY` — KMS key id/ARN to enable SSE-KMS (takes precedence over `TRACEABILITY_S3_SSE`)

Minimal IAM policy (if using access keys)
---------------------------------------

The IAM principal used by the secrets only needs permission to put objects into the specified prefix. Example minimal policy:

```json
{
  "Version": "2012-10-17",
  "Statement": [
    {
      "Effect": "Allow",
      "Action": [
        "s3:PutObject",
        "s3:PutObjectAcl"
      ],
      "Resource": "arn:aws:s3:::YOUR_BUCKET_NAME/traceability/*"
    }
  ]
}
```

Notes
-----
- Uploaded objects are placed under: `s3://<TRACEABILITY_S3_BUCKET>/<TRACEABILITY_S3_PREFIX>/<ref>-<UTC-timestamp>/`.
- The workflow uploads objects with `--acl bucket-owner-full-control` by default.
- Server-side encryption options:
  - Set `TRACEABILITY_S3_SSE=AES256` to use SSE-S3.
  - Set `TRACEABILITY_KMS_KEY` to a KMS key ARN/id to use SSE-KMS.
# Traceability checker

This repository includes a simple, dependency-free traceability checker at `scripts/traceability_check.py`.

What it does
- Scans `reqs/` YAML files and extracts common link patterns (`links:`, `ref:`, `satisfied-by:`).
- Produces `artifacts/traceability-matrix.csv` with columns: `req_id,target_id,link_type,source_file`.

Quick run
```bash
python3 scripts/traceability_check.py reqs --output artifacts/traceability-matrix.csv
```

CI
- Add a pipeline step that runs the checker and fails on `--fail-on-unlinked` if you want to prevent isolated requirements.

S3 uploads (optional)
---------------------

The workflow optionally uploads the generated CSV and Markdown report to S3 when repository secrets are configured. To enable this feature, add the following GitHub repository secrets:

- `AWS_ACCESS_KEY_ID` — IAM user access key id
- `AWS_SECRET_ACCESS_KEY` — IAM user secret access key
- `TRACEABILITY_S3_BUCKET` — the target S3 bucket name

Optional secrets/values:

- `AWS_REGION` — AWS region (defaults to `us-east-1`)
- `TRACEABILITY_S3_PREFIX` — key prefix inside the bucket (defaults to `traceability`)

Minimal IAM policy
------------------

The IAM principal used by the secrets only needs permission to put objects into the specified prefix. Example minimal policy:

```json
{
  "Version": "2012-10-17",
  "Statement": [
    {
      "Effect": "Allow",
      "Action": [
        "s3:PutObject",
        "s3:PutObjectAcl"
      ],
      "Resource": "arn:aws:s3:::YOUR_BUCKET_NAME/traceability/*"
    }
  ]
}
```

Notes
-----
- Uploaded objects are placed under: `s3://<TRACEABILITY_S3_BUCKET>/<TRACEABILITY_S3_PREFIX>/<ref>-<UTC-timestamp>/`.
- The workflow uploads objects with `--acl bucket-owner-full-control`. If you require server-side encryption or different ACLs, tell me and I can update the workflow to add `--sse AES256` or a KMS key.
Traceability checker
====================

This repository includes a simple, dependency-free traceability checker at `scripts/traceability_check.py`.

What it does
- Scans `reqs/` YAML files and extracts common link patterns (`links:`, `ref:`, `satisfied-by:`).
- Produces `artifacts/traceability-matrix.csv` with columns: `req_id,target_id,link_type,source_file`.

Quick run
```
python3 scripts/traceability_check.py reqs --output artifacts/traceability-matrix.csv
```

CI
-- Add a pipeline step that runs the checker and fails on `--fail-on-unlinked` if you want to prevent isolated requirements.
