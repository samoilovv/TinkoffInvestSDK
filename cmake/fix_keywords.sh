#!/bin/bash

#sed -i -- 's/namespace public {/namespace public_ {/g' $1/*.pb.h $1/*.pb.cc
#sed -i -- 's/tinkoff::public::/tinkoff::public_::/g' $1/*.pb.h $1/*.pb.cc

#sed -i -- 's/namespace public {/namespace public_ {/g' $OUTPUT_PB_SOURCE $OUTPUT_PB_HEADER $OUTPUT_GRPC_SOURCE $OUTPUT_GRPC_HEADER
#sed -i -- 's/tinkoff::public::/tinkoff::public_::/g' $OUTPUT_PB_SOURCE $OUTPUT_PB_HEADER $OUTPUT_GRPC_SOURCE $OUTPUT_GRPC_HEADER

sed -i -- 's/namespace public {/namespace public_ {/g' $1 $2 $3 $4
sed -i -- 's/tinkoff::public::/tinkoff::public_::/g' $1 $2 $3 $4
