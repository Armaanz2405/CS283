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
  export TEST_PORT=7890
  ./dsh -s -p "$TEST_PORT" > /dev/null 2>&1 &
  SERVER_PID=$!
  sleep 1
}

teardown() {
  if ps -p $SERVER_PID > /dev/null 2>&1; then
    kill $SERVER_PID
  fi
}

@test "Remote shell executes a simple command (echo hello)" {
  run bash -c 'printf "echo hello\nexit\n" | ./dsh -c -p "$TEST_PORT"'
  [ "$status" -eq 0 ]
  [[ "$output" == *"hello"* ]]
}

@test "Remote shell built-in command dragon outputs art" {
  run bash -c 'printf "dragon\nexit\n" | ./dsh -c -p "$TEST_PORT"'
  [ "$status" -eq 0 ]
  [[ "$output" == *"@%%%%"* ]]
}

@test "Remote shell pipeline command transforms output" {
  run bash -c 'printf "echo hello | tr a-z A-Z\nexit\n" | ./dsh -c -p "$TEST_PORT"'
  [ "$status" -eq 0 ]
  [[ "$output" == *"HELLO"* ]]
}

@test "Remote shell built-in cd changes directory" {
  run bash -c 'printf "cd /tmp\npwd\nexit\n" | ./dsh -c -p "$TEST_PORT"'
  [ "$status" -eq 0 ]
  [[ "$output" == *"/tmp"* ]]
}

@test "Remote shell stop-server command stops the server" {
  run bash -c 'printf "stop-server\n" | ./dsh -c -p "$TEST_PORT"'
  [ "$status" -eq 0 ]
}

