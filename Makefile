VERSION := 1.0.8

docker:
	@docker build -t alextanhongpin/nginx-reason .

tag:
	@docker tag alextanhongpin/nginx-reason:latest alextanhongpin/nginx-reason:${VERSION} 

push:
	@docker push alextanhongpin/nginx-reason:${VERSION} 

production:
	make docker && make tag && make push