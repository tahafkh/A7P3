CC=g++
STD=-std=c++11 -Wall -pedantic
CF=$(STD)
BUILD_DIR=build
TEMPLATE_DIR=.template
Obj_files = $(BUILD_DIR)/UtripService.o $(BUILD_DIR)/main.o $(BUILD_DIR)/Handlers.o $(BUILD_DIR)/response.o $(BUILD_DIR)/request.o $(BUILD_DIR)/utilities.o $(BUILD_DIR)/server.o $(BUILD_DIR)/route.o $(BUILD_DIR)/template_parser.o $(BUILD_DIR)/User.o $(BUILD_DIR)/SameType.o $(BUILD_DIR)/Room.o $(BUILD_DIR)/Reserve.o $(BUILD_DIR)/Period.o $(BUILD_DIR)/Hotel.o $(BUILD_DIR)/StarFilter.o $(BUILD_DIR)/NotFound.o $(BUILD_DIR)/NotEnough.o $(BUILD_DIR)/InsufficientRatings.o $(BUILD_DIR)/Comment.o $(BUILD_DIR)/BadRequest.o $(BUILD_DIR)/PermissionDenied.o
all: $(BUILD_DIR) utripserver.out

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)
$(BUILD_DIR)/User.o: utrip-details/User.cpp utrip-details/User.hpp utrip-details/Reserve.hpp utrip-details/NotFound.hpp utrip-details/InsufficientRatings.hpp
	${CC} $(CF) -c utrip-details/User.cpp -o $(BUILD_DIR)/User.o
$(BUILD_DIR)/SameType.o: utrip-details/SameType.cpp utrip-details/SameType.hpp utrip-details/Filter.hpp utrip-details/Rating.hpp
	${CC} $(CF) -c utrip-details/SameType.cpp -o $(BUILD_DIR)/SameType.o
$(BUILD_DIR)/Room.o: utrip-details/Room.cpp utrip-details/Room.hpp utrip-details/Period.hpp utrip-details/Reserve.hpp
	${CC} $(CF) -c utrip-details/Room.cpp -o $(BUILD_DIR)/Room.o
$(BUILD_DIR)/Reserve.o: utrip-details/Reserve.cpp utrip-details/Reserve.hpp utrip-details/Room.hpp utrip-details/Period.hpp 
	${CC} $(CF) -c utrip-details/Reserve.cpp -o $(BUILD_DIR)/Reserve.o
$(BUILD_DIR)/Period.o: utrip-details/Period.cpp utrip-details/Period.hpp 
	${CC} $(CF) -c utrip-details/Period.cpp -o $(BUILD_DIR)/Period.o
$(BUILD_DIR)/Hotel.o: utrip-details/Hotel.cpp utrip-details/Hotel.hpp utrip-details/Rating.hpp utrip-details/Room.hpp utrip-details/Reserve.hpp utrip-details/Comment.hpp utrip-details/SameType.hpp utrip-details/NotEnough.hpp
	${CC} $(CF) -c utrip-details/Hotel.cpp -o $(BUILD_DIR)/Hotel.o
$(BUILD_DIR)/StarFilter.o: utrip-details/StarFilter.cpp utrip-details/StarFilter.hpp utrip-details/Filter.hpp utrip-details/Hotel.hpp
	${CC} $(CF) -c utrip-details/StarFilter.cpp -o $(BUILD_DIR)/StarFilter.o
$(BUILD_DIR)/Comment.o: utrip-details/Comment.cpp utrip-details/Comment.hpp utrip-details/User.hpp
	${CC} $(CF) -c utrip-details/Comment.cpp -o $(BUILD_DIR)/Comment.o
$(BUILD_DIR)/InsufficientRatings.o: utrip-details/InsufficientRatings.cpp utrip-details/InsufficientRatings.hpp
	${CC} $(CF) -c utrip-details/InsufficientRatings.cpp -o $(BUILD_DIR)/InsufficientRatings.o
$(BUILD_DIR)/PermissionDenied.o: utrip-details/PermissionDenied.cpp utrip-details/PermissionDenied.hpp
	${CC} $(CF) -c utrip-details/PermissionDenied.cpp -o $(BUILD_DIR)/PermissionDenied.o
$(BUILD_DIR)/NotFound.o: utrip-details/NotFound.cpp utrip-details/NotFound.hpp
	${CC} $(CF) -c utrip-details/NotFound.cpp -o $(BUILD_DIR)/NotFound.o
$(BUILD_DIR)/NotEnough.o: utrip-details/NotEnough.cpp utrip-details/NotEnough.hpp
	${CC} $(CF) -c utrip-details/NotEnough.cpp -o $(BUILD_DIR)/NotEnough.o
$(BUILD_DIR)/BadRequest.o: utrip-details/BadRequest.cpp utrip-details/BadRequest.hpp
	${CC} $(CF) -c utrip-details/BadRequest.cpp -o $(BUILD_DIR)/BadRequest.o
$(BUILD_DIR)/template_parser.o: utils/template_parser.cpp utils/template_parser.hpp utils/request.cpp utils/request.hpp utils/utilities.hpp utils/utilities.cpp
	$(CC) $(CF) -c utils/template_parser.cpp -o $(BUILD_DIR)/template_parser.o
$(BUILD_DIR)/response.o: utils/response.cpp utils/response.hpp utils/include.hpp
	$(CC) $(CF) -c utils/response.cpp -o $(BUILD_DIR)/response.o
$(BUILD_DIR)/request.o: utils/request.cpp utils/request.hpp utils/include.hpp utils/utilities.hpp
	$(CC) $(CF) -c utils/request.cpp -o $(BUILD_DIR)/request.o
$(BUILD_DIR)/utilities.o: utils/utilities.cpp utils/utilities.hpp
	$(CC) $(CF) -c utils/utilities.cpp -o $(BUILD_DIR)/utilities.o
$(BUILD_DIR)/server.o: server/server.cpp server/server.hpp server/route.hpp utils/utilities.hpp utils/response.hpp utils/request.hpp utils/include.hpp utils/template_parser.hpp utils/template_parser.cpp
	$(CC) $(CF) -c server/server.cpp -o $(BUILD_DIR)/server.o
$(BUILD_DIR)/route.o: server/route.cpp server/route.hpp utils/utilities.hpp utils/response.hpp utils/request.hpp utils/include.hpp
	$(CC) $(CF) -c server/route.cpp -o $(BUILD_DIR)/route.o
$(BUILD_DIR)/Handlers.o: utrip-service/Handlers.cpp utrip-service/Handlers.hpp utrip-service/UtripService.hpp server/server.hpp utils/utilities.hpp utils/response.hpp utils/request.hpp utils/include.hpp
	$(CC) $(CF) -c utrip-service/Handlers.cpp -o $(BUILD_DIR)/Handlers.o
$(BUILD_DIR)/UtripService.o: utrip-service/UtripService.cpp utrip-service/UtripService.hpp server/server.hpp utils/utilities.hpp utils/response.hpp utils/request.hpp utils/include.hpp
	$(CC) $(CF) -c utrip-service/UtripService.cpp -o $(BUILD_DIR)/UtripService.o
$(BUILD_DIR)/main.o: utrip-service/main.cpp utrip-service/UtripService.hpp server/server.hpp utils/utilities.hpp utils/response.hpp utils/request.hpp utils/include.hpp
	$(CC) $(CF) -c utrip-service/main.cpp -o $(BUILD_DIR)/main.o
utripserver.out: $(Obj_files)
	$(CC) $(CF) $(Obj_files) -o utripserver.out

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR) $(TEMPLATE_DIR) *.o *.out &> /dev/null
