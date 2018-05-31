VERSION := 1.0.4

docker:
	@docker build -t alextanhongpin/nginx-reason .

tag:
	@docker tag alextanhongpin/nginx-reason:latest alextanhongpin/nginx-reason:${VERSION} 

push:
	@docker push alextanhongpin/nginx-reason:${VERSION} 
