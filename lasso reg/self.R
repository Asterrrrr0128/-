library(mvtnorm)
library(corrplot)
library(glmnet)

rm(list = ls())

#data generate model
p1 <- p0 <- 10 		#number of possible predictors
b0 <- 0
set.seed(123)
c1 <- sample(c(-1, 0, 1), size = p0, prob = c(0.2, 0.6, 0.2), replace = TRUE)
set.seed(123)
c2 <- runif(p0, 2, 3)
b1 <- round(c1 * c2, 2)
names(b2) <- paste0("g", 1:p0)
b1.true <- b1

#simulated data set
N0 <- 2000
set.seed(123)
X0 <- matrix(runif(N0 * p0, -3, 3), N0, p0)
colnames(X0) < paste0("g", 1:p0)
y0 <- b0 + X0 %*% b1	#data of target variable
colnames(y0) <- "g0"

set.seed(123)
e0 <- rnorm(N0, 0, 0.05)
y0 <- y0 + e0
z0 <- cbind(y0, X0)

#train & test set
#train
N1 <- 20
set.seed(123)
I1 <- sample(1:N0, N1)
Z1 <- Z0[I1, ]

#test
I2 <- setdiff(1:N0, I1)
Z2 <- Z0[I2, ]

#specify the response(target) variable
V1 <- colnames(Z1)
T1 <- c("g0") 		#target variable
P0 <- setdiff(V1, T1) 	#potential predictor


#input output data set
y1 <- Z1[ ,T1] 		#data of target var
X1 <- Z1[ ,P0]
y1.c <- scale(y1, center = T , scale = F)
X1.c <- scale(X1, center = T , scale = F)

y2 <- Z2[ ,T1]  		
X2 <- Z2[ ,P0]

head(Z1, 5)


#least absolute shrinkage and selection operator
#obj function : penalized rss function of b1

f1 <- function(b1){
	a1 <- sum((y1.c - X1.c %*% b1)^2) + ld * sum(abs(b1))
	return(a1)
}


#lasso estimate of b1, b0
ld <- 1
p1 <- ncol(X1.c)
ans <- optim(rep(0, p1), f1, method = "BFGS")

b1.1 <- ans$par
b1.1 <- matrix(b1.r, ncol = 1)
b0.1 <- mean(y1 - X1 %*% b1.r)

mean((y1.c - X1.c %*% b1.1)^2)
mean((y1 - b0.1 - X1 %*% b1.1)^2)


#soft-thresholding function
st <- function(x, ld){
	ifelse(x > ld, x - ld,
	  ifelse(x < -ld, x + ld, 0)
	)
}

st.1 <- function(x){
	return(st(x, ld = 1))
}
curve(st.1, -3, 3, col = "red",
  main = "Soft-thresholding function"
)


#implementing lasso using the CD method
ite <<- 100  
tol <- 10^-6  #tolerance level of convergence

b1 <- rep(0, p1)
cinverge <- FALSE
for ( i in 1:ite ){
	b1.0 <- b1
	for (j in 1:p1){
	  X_j <- X1[ ,j]
	  r_j <- y1 - (X1[, -j] %*% b1[-j])
	  Xr_j <- sum(X_j * r_j)
	  XX_j <- sum(X_j ^ 2)
	  b1[j] <- st(Xr_j, ld) / XX_j
	}
	if(max(abs(b1 - b1.0)) < tol){
	  converge <- TRUE
	  break
	}
	
}
names(b1) <- colnames(X1)
i
plot(b1, pch = 20, col = 2)
b1; c(b1.1)


#user define function of lasoo
las <- function(X1, y1, ld, ite, tol){
	X.1 <- scale(X1, center = TRUE, scale = TRUE)
	y.1 <- scale(y1, center = TRUE, scale = FALSE)

	st <- function(x, ld){
	  ifelse(x > ld, x - ld,
	    ifelse(x < -ld, x + ld, 0)
	  )
	)#st
	p <- ncol(X1)	
	b1 <- rep(0, p)

	cinvergence <- FALSE
	for ( i in 1:ite ){
	  b1.0 <- b1
	  for (j in 1:p){
	    X_j <- X1[ ,j]
	    r_j <- y1 - (X.1[, -j] %*% b1[-j])
	    Xr_j <- sum(X_j * r_j)
	    XX_j <- sum(X_j ^ 2)
	    b1[j] <- st(Xr_j, ld) / XX_j
	  }#for
	  if(max(abs(b1 - b1.0)) < tol){
	    converge <- TRUE
	    break
	  }#if
	}#for
	names(b1) <- colnames(X1)
	b0 <- mean(y1 - X1 %*% b1)
	return(list(b0 = b0, b1 = b1, convergence = convergence))

}#las	

#prediction
pred.las <- function(X2, X1, b0, b1){
	X2.t <- scale(X2, center = colMeans(X1), scale = apply(X1, 2, sd))
	py2 <- b0 + X2.t %*% b1
	return (py2)
}#pred 

#fit thr lasso to data
ld <- 1
ite <- 100
tol <- 10^-6
a1 <- las(X1, y1, ld, ite, tol)

b0 <- a1$b0
b1 <- a1$b1


plot(b1.true, ylim = c(-3, 3), cex = 2, pch = 20, col = "red")
points(1:p0, b1, cex = 1, pch = 20, col = "skyblue")

py1 <- pred.las(X1, X1, b0, b1)
s2.ld <- mean((y1 - py1)^2)
s2.ld

df.reg <- sum(abs(b1) > 10^-6)
df.reg

aic <- N1 * log(s2.ld) + 2 * df.reg
aic
gcv <- s2.ld * 1/(1 - df.reg/N1) ^ 2
gcv


py2 <- pred.las(X2, X, b0, b1)
err.te <- mean((y2 - py2) ^ 2)
err.te

#sol path  pf b1
Ld <- seq(1, 300, length = 10)
Gcv <- B1 <- NULL
for(i1 in 1:length(Ld)){
	ld <- Ld[i1]
	a1 <- las(X1, y1, ld, ite, tol)
	b0 <- a1$b0
	b1 <- a1$b1
	py1 <- pred.las(X1, X1, b0, b1)
	s2.ld <- mean((y1 - py1)^2)
	df.reg <- sum(abs(b1) > 10^-6)
	gcv <- s2.ld * 1/(1 - df.reg/nrow*(X1)) ^ 2
	B1 <- rbind(B1, b1)
	Gcv <- c(Gcv, gcv)
}

matplot(B1, type = "1", xlab = "lambda", ylab = "b1", main = "sol path  pf b1")
ld.gcv <- Ld[which.min(Gcv)]
plot(Ld, Gcv, pch = 20, type = "b", xlab = "lambda", ylab = "Gcv", main = "GCV VS lambda")

abline (v = ld.gcv, col = 2, lty = 2)


#glmnet::glmnet
Fit.glmnet <- glmnet::glmnet(
  x = scale(X1, center = TRUE, scale = TRUE),
  y = scale(y1, center = TRUE, scale = FALSE),
  alpha = 1,
  nlambda = 100,
  lambda.min.ratio = ifelse(p1 < N1, 0.01, 1e-04),
  intercept = FALSE,
  thresh = 1e-07
)
names(Fit.glmnet)
plot(Fit.glmnet)
Ld.glmnet <- Fit.glmnet$lambda
plot(Ld.glmnet)
B1.glmnet <- as.matrix(Fit.glmnet$beta)