# Darknet Makefile

########## 매크로 정의 ##########


GPU=0 # CUDA 설치되었으면 1, 안되었으면 0

CUDNN=0 # CUDNN 설치되었으면 1, 안되었으면 0

OPENCV=0 # OPENCV 설치되었으면 1, 안되었으면 0

OPENMP=0 # OPENMP 설치되었으면 1, 안되었으면 0

DEBUG=0 # DEBUG 모드 사용시 1, 아니면 0

# macro 여러줄로 정의

ARCH= -gencode arch=compute_30,code=sm_30 \

      -gencode arch=compute_35,code=sm_35 \

      -gencode arch=compute_50,code=[sm_50,compute_50] \

      -gencode arch=compute_52,code=[sm_52,compute_52]

#      -gencode arch=compute_20,code=[sm_20,sm_21] \ This one is deprecated?



# This is what I use, uncomment if you know your arch and want to specify

# ARCH= -gencode arch=compute_52,code=compute_52



VPATH=./src/:./examples

SLIB=libdarknet.so # 리눅스/동적 라이브러리

ALIB=libdarknet.a # 리눅스/정적 라이브러리 

EXEC=darknet # 실행파일(결과)

OBJDIR=./obj/ # 오브젝트 디렉토리



CC=gcc # C 컴파일러

CPP=g++ # C++ 컴파일러

NVCC=nvcc # CUDA 컴파일러

AR=ar # Archive 

ARFLAGS=rcs # Archive 옵션

OPTS=-Ofast

LDFLAGS= -lm -pthread # 링커 옵션

COMMON= -Iinclude/ -Isrc/ 

CFLAGS=-Wall -Wno-unused-result -Wno-unknown-pragmas -Wfatal-errors -fPIC # 컴파일 옵션



ifeq ($(OPENMP), 1) # OPENMP가 설치되어있으면 매크로 추가 정의

CFLAGS+= -fopenmp

endif



ifeq ($(DEBUG), 1) # 디버그 모드이면 매크로 추가 정의

OPTS=-O0 -g

endif



CFLAGS+=$(OPTS)



ifeq ($(OPENCV), 1) # OPENCV가 설치되어있으면 매크로 추가 정의

COMMON+= -DOPENCV 

CFLAGS+= -DOPENCV

LDFLAGS+= `pkg-config --libs opencv` -lstdc++

COMMON+= `pkg-config --cflags opencv` 

endif



ifeq ($(GPU), 1) # CUDA가 설치되어있으면 매크로 추가 정의

COMMON+= -DGPU -I/usr/local/cuda/include/

CFLAGS+= -DGPU

LDFLAGS+= -L/usr/local/cuda/lib64 -lcuda -lcudart -lcublas -lcurand

endif



ifeq ($(CUDNN), 1) # CUDNN이 설치되어있으면 매크로 추가 정의

COMMON+= -DCUDNN 

CFLAGS+= -DCUDNN

LDFLAGS+= -lcudnn

endif


# 오브젝트 파일 목록
OBJ=gemm.o utils.o cuda.o deconvolutional_layer.o convolutional_layer.o list.o image.o activations.o im2col.o col2im.o blas.o crop_layer.o dropout_layer.o maxpool_layer.o softmax_layer.o data.o matrix.o network.o connected_layer.o cost_layer.o parser.o option_list.o detection_layer.o route_layer.o upsample_layer.o box.o normalization_layer.o avgpool_layer.o layer.o local_layer.o shortcut_layer.o logistic_layer.o activation_layer.o rnn_layer.o gru_layer.o crnn_layer.o demo.o batchnorm_layer.o region_layer.o reorg_layer.o tree.o  lstm_layer.o l2norm_layer.o yolo_layer.o iseg_layer.o image_opencv.o

EXECOBJA=captcha.o lsd.o super.o art.o tag.o cifar.o go.o rnn.o segmenter.o regressor.o classifier.o coco.o yolo.o detector.o nightmare.o instance-segmenter.o darknet.o


ifeq ($(GPU), 1) # CUDA가 설치되어있으면 매크로 추가 정의

LDFLAGS+= -lstdc++

OBJ+=convolutional_kernels.o deconvolutional_kernels.o activation_kernels.o im2col_kernels.o col2im_kernels.o blas_kernels.o crop_layer_kernels.o dropout_layer_kernels.o maxpool_layer_kernels.o avgpool_layer_kernels.o # 오브젝트 목록에 추가

endif



EXECOBJ = $(addprefix $(OBJDIR), $(EXECOBJA)) # 접두어+문자열 $(OBJDIR)$(EXECOBJA)

OBJS = $(addprefix $(OBJDIR), $(OBJ)) # 접두어+문자열 $(OBJDIR)$(OBJ)

DEPS = $(wildcard src/*.h) Makefile include/darknet.h # src 디렉토리 안에 있는 모든 헤더파일(.h) 매크로 정의



########## 명령어 정의 ##########


all: obj backup results $(SLIB) $(ALIB) $(EXEC) # 여러개의 실행파일 생성

#all: obj  results $(SLIB) $(ALIB) $(EXEC)





$(EXEC): $(EXECOBJ) $(ALIB) # target(실행파일) : dependency(EXECOBJ, ALIB)

	$(CC) $(COMMON) $(CFLAGS) $^ -o $@ $(LDFLAGS) $(ALIB) # command($(CC) $(COMMON) (dependency 전체 목록) -o (target) $(LDFLAGS) $(ALIB))



$(ALIB): $(OBJS) # target(ALIB) : dependency(OBJS)

	$(AR) $(ARFLAGS) $@ $^ # command($(AR) $(ARFLAGS) (target) (dependency 전체 목록)



$(SLIB): $(OBJS) # target(SLIB) : dependency(OBJS)

	$(CC) $(CFLAGS) -shared $^ -o $@ $(LDFLAGS) # command($(CC) $(CFLAGS) -shared (dependency 전체 목록) -o (target) $(LDFLAGS))



$(OBJDIR)%.o: %.cpp $(DEPS) # target($(OBJDIR)%.o) : dependency(%.cpp $(DEPS))

	$(CPP) $(COMMON) $(CFLAGS) -c $< -o $@ # command($(CPP) $(COMMON) $(CFLAGS) -c (첫번째 dependency: %.cpp) -o (target))



$(OBJDIR)%.o: %.c $(DEPS) # target($(OBJDIR)%.o) : dependency(%.c $(DEPS))

	$(CC) $(COMMON) $(CFLAGS) -c $< -o $@ # command($(CC) $(COMMON) $(CFLAGS) -c (첫번째 dependency: %.c) -o (target))



$(OBJDIR)%.o: %.cu $(DEPS) # target($(OBJDIR)%.o) : dependency(%.cu $(DEPS))

	$(NVCC) $(ARCH) $(COMMON) --compiler-options "$(CFLAGS)" -c $< -o $@ # command($(NVCC) $(ARCH) $(COMMON) --compiler-options "$(CFLAGS)" -c (첫번째 dependency: %.cu) -o (target))



obj: # target(obj)

	mkdir -p obj # command(obj 이름의 딕렉토리 생성, 상위 경로 포함)

backup: # target(backup)

	mkdir -p backup # command(backup 이름의 딕렉토리 생성, 상위 경로 포함)

results: # target(results)

	mkdir -p results # command(result 이름의 딕렉토리 생성, 상위 경로 포함)



.PHONY: clean # Phony target (실제 동일한 파일이름과 충동 해결 / 성능 향상)



clean: # makefile 실행 후 파일/디렉토리 제거

	rm -rf $(OBJS) $(SLIB) $(ALIB) $(EXEC) $(EXECOBJ) $(OBJDIR)/* # 파일 제거, 디렉토리일 경우 하위 파일까지 제거

