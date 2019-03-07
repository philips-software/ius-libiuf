#!/usr/bin/env bash
ScriptPath="$( cd "$(dirname "$0")" ; pwd -P )"

Script=$ScriptPath/code_coverage.sh
[[ -f $Script ]] || (printf "Error: $Script not found\n" >&2  && exit 1)
$Script
Script=$ScriptPath/static_code_analysis.sh
[[ -f $Script ]] || (printf "Error: $Script not found\n" >&2  && exit 1)
$Script
Script=$ScriptPath/memory_leak_detection.sh xml
[[ -f $Script ]] || (printf "Error: $Script not found\n" >&2  && exit 1)
$Script
sonar-scanner -Dsonar.login=${SONAR_LOGIN}  -Dsonar.host.url=https://sonarc.ta.philips.com/

