# Security Policy

## Supported Versions

| Version            | Supported with security fixes |
|--------------------|-------------------------------|
| 1.0.x              | Yes                           |
| 29.99.x and older pre-release builds | No — please upgrade to the latest 1.0.x release |

## Reporting a Vulnerability

Please **do not** open a public GitHub issue for security problems
(consensus bugs, remote crashes, wallet key exposure, network-level denial of
service, etc.).

Report them privately through GitHub's private vulnerability reporting:

  https://github.com/COINWOW/COINWOW/security/advisories/new

Include, where possible: affected version or commit, platform, steps to
reproduce, and the impact you observed. You will receive an acknowledgement
as soon as the report has been reviewed.

COINWOW Core is derived from Bitcoin Core. If you believe a vulnerability
also affects upstream Bitcoin Core, please additionally report it to the
Bitcoin Core project following
[its own security policy](https://github.com/bitcoin/bitcoin/blob/master/SECURITY.md).
Vulnerabilities in the vendored `libsecp256k1` library should be reported
upstream as described in [`src/secp256k1/SECURITY.md`](src/secp256k1/SECURITY.md).
