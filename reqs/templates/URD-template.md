---
id: URD-template
header: "Short requirement title here"
text: |
  "Detailed requirement statement here"

# TSF Type: Assertion (Both a Request and a Claim)

ASIL: "ASIL level here (QM/A/B/C/D)"
verification_method: "Verification method here"

# Links: Connects to child Assertion
children:
  - id: SRD-<CHILD_REF>

reviewers:
  - name: "<REVIEWER_NAME>"
    email: "<REVIEWER_EMAIL>"
reviewed: ''  # Manually fill on PR approval (YYYY-MM-DD - Approved by Name <email>)

# Evidence Linking (use 'references:', NOT deprecated 'artifact:')
references:
  - type: "file"
    path: <relative/path/to/artifact>  # Path from repo root
  # Add more references as needed
  # - type: "url"
  #   path: <https://github.com/SEAME-pt/Team04_DrivaPi/pull/XX>

# Manual SME Score
score:
  <SME_ID>: <0.0-1.0>  # SME confidence score (1.0=certain true, 0.5=uncertain, 0.0=certain false)
  # Example:
  # ProductOwner: 0.95

# Optional: Automated Validation
# evidence:
#   type: <validator_name>
#   references:
#     - type: "file"
#       path: <path/to/artifact/to/validate>
#   configuration: # input parameters for the validator
#     <param>: <value>

active: true
derived: false
normative: true
level: 1.0  # 1.0=URD, 2.0=SRD, 3.0=SWD, 4.0=LLTC
---
<STATEMENT>
