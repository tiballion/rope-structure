#!/bin/bash

# Execute the Make command
make

# Run the my_program executable
./my_program

# Creates the graph representation
dot -Tpng rope_graph.dot -o rope_graph.png
dot -Tpng rope_graph_after_insert.dot -o rope_graph_after_insert.png