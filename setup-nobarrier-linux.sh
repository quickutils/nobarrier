NAME=$(whoami)

main() {
	build_nobarrier
	create_service_file
	enable_service
}

build_nobarrier() {
	echo Building build/nobarrier...
	mkdir -p $PWD/build
	gcc $PWD/nobarrier.c -o $PWD/build/nobarrier 
}

create_service_file() {
	echo Writing the service file build/nobarrier.service...
	echo [Unit] > $PWD/build/nobarrier.service
	echo Description=Auto detect and connect to barrier server >> $PWD/build/nobarrier.service
	echo After=network.target >> $PWD/build/nobarrier.service
	echo StartLimitIntervalSec=0 >> $PWD/build/nobarrier.service
	echo [Service] >> $PWD/build/nobarrier.service
	echo Type=simple >> $PWD/build/nobarrier.service
	echo Restart=on-failure >> $PWD/build/nobarrier.service
	echo RestartSec=1 >> $PWD/build/nobarrier.service
	echo User=$NAME >> $PWD/build/nobarrier.service
	echo ExecStart=$PWD/build/nobarrier $NAME >> $PWD/build/nobarrier.service
	echo  >> $PWD/build/nobarrier.service
	echo [Install] >> $PWD/build/nobarrier.service
	echo WantedBy=multi-user.target >> $PWD/build/nobarrier.service
}

enable_service() {
	systemctl enable $PWD/build/nobarrier.service
	systemctl start $PWD/build/nobarrier.service
}

main $*