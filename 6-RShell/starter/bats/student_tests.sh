#!/usr/bin/env bats

# File: student_tests.sh
# 
# Create your unit tests suit in this file

@test "Example: check ls runs without errors" {
    run ./dsh <<EOF                
ls
EOF

    # Assertions
    [ "$status" -eq 0 ]
}

setup() {
  ./dsh -s -p 5678 &
  server_pid=$!
  sleep 1
}

teardown() {
  kill $server_pid 2>/dev/null
}

@test "Remote Shell: echo command" {
  run ./dsh -c -p 5678 <<EOF
echo hello remote
EOF
  [ "$status" -eq 0 ]
  echo "$output" | grep "hello remote"
}

@test "Remote Shell: Built-in exit command" {
  run ./dsh -c -p 5678 <<EOF
exit
EOF
  [ "$status" -eq 0 ]
}

@test "Remote Shell: Command with pipes" {
  run ./dsh -c -p 5678 <<EOF
ls | grep dsh
EOF
  [ "$status" -eq 0 ]
  echo "$output" | grep "dsh"
}

@test "Remote Shell: Built-in dragon command" {
  run ./dsh -c -p 5678 <<EOF
dragon
EOF
  [ "$status" -eq 0 ]
  echo "$output" | grep -i "dragon"
}

@test "Remote Shell: Built-in stop-server command" {
  run ./dsh -c -p 5678 <<EOF
stop-server
EOF
  [ "$status" -eq 0 ]
  sleep 1
  run nc -z 127.0.0.1 5678
  [ "$status" -ne 0 ]
}
