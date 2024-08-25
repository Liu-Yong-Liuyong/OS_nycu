#!/bin/bash

path_print=false
path=()
# Read parent pid and child pid
while [ "$#" -gt 0 ]; do
  case "$1" in
    --parent)
      parent="$2"
      shift 2
      ;;
    --child)
      child="$2"
      shift 2
      ;;
    --path)
      path_print=true
      shift
      ;;
    *)
      echo "Unknown parameter passed: $1"
      exit 1
      ;;
  esac
done

# Check if parent or child is empty
if [ -z "$parent" ] || [ -z "$child" ]; then
  echo "Usage: $0 --parent PARENT_PID --child CHILD_PID [--path]"
  exit 1
fi


# check if a process is an ancestor of another process
is_ancestor() {
  local target_pid="$1"
  local current_pid="$2"

  while [ "$current_pid" -ne 0 ]; do
    if [ "$current_pid" -eq "$target_pid" ]; then
      path+=("$current_pid") #
      return 0  # The target process is an ancestor
    fi
    path+=("$current_pid") #
    current_pid=$(ps -o ppid= -p "$current_pid" | tr -d ' ')
    
  done

  return 1  # The target process is not an ancestor
}

# check if the parent is an ancestor of the child


if is_ancestor "$parent" "$child"; then
  if [ "$path_print" = true ]; then
    echo "Yes"
    for ((i=${#path[@]}-1; i>=0; i--)); do
    	echo -n "${path[i]}"
   	if [ "$i" -ne 0 ]; then
      	   echo -n " -> "
    	else
      	   echo  # Add a newline at the end
    	fi
    done
  else
    echo "Yes"
  fi
else
  echo "No"
fi
