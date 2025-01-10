# === Data Generation ===
# 生成模擬數據集
p1 <- p0 <- 10  # 特徵數量
b0 <- 0  # 截距
set.seed(123)
c1 <- sample(c(-1, 0, 1), size = p0, prob = c(0.2, 0.6, 0.2), replace = TRUE)  # 特徵方向
set.seed(123)
c2 <- runif(p0, 2, 3)  # 特徵強度
b1 <- round(c1 * c2, 2)  # 真實係數
names(b1) <- paste0("g", 1:p0)
b1.true <- b1  # 保存真實係數以供後續比較

# 生成目標變量與特徵矩陣
N0 <- 2000  # 樣本總數
set.seed(123)
X0 <- matrix(runif(N0 * p0, -3, 3), N0, p0)  # 特徵矩陣
colnames(X0) <- paste0("g", 1:p0)
y0 <- b0 + X0 %*% b1  # 目標變量
set.seed(123)
e0 <- rnorm(N0, 0, 0.05)  # 添加噪聲
y0 <- y0 + e0
Z0 <- cbind(y0, X0)  # 結合特徵與目標變量

# === Train & Test Split ===
# 將數據劃分為訓練集與測試集
N1 <- 20  # 訓練集大小
set.seed(123)
I1 <- sample(1:N0, N1)  # 訓練集索引
Z1 <- Z0[I1, ]  # 訓練集
I2 <- setdiff(1:N0, I1)  # 測試集索引
Z2 <- Z0[I2, ]  # 測試集

# === 定義目標變量與特徵變量 ===
V1 <- colnames(Z1)  # 獲取列名
T1 <- c("y0")  # 目標變量
P0 <- setdiff(V1, T1)  # 特徵變量
y1 <- Z1[, T1]  # 訓練集目標變量
X1 <- Z1[, P0]  # 訓練集特徵
y1.c <- scale(y1, center = TRUE, scale = FALSE)  # 中心化目標變量
X1.c <- scale(X1, center = TRUE, scale = TRUE)  # 標準化特徵

y2 <- Z2[, T1]  # 測試集目標變量
X2 <- Z2[, P0]  # 測試集特徵

# === Lasso Regression Implementation ===
# Lasso 的目標函數
f1 <- function(b1) {
  a1 <- sum((y1.c - X1.c %*% b1)^2) + ld * sum(abs(b1))  # 最小化目標
  return(a1)
}

# 使用 BFGS 方法進行優化
ld <- 1  # 正則化參數
p1 <- ncol(X1.c)
ans <- optim(rep(0, p1), f1, method = "BFGS")

# 優化結果
b1.1 <- ans$par  # 係數估計值
b1.1 <- matrix(b1.1, ncol = 1)
b0.1 <- mean(y1 - X1 %*% b1.1)  # 截距估計值

# 訓練誤差
train_error <- mean((y1.c - X1.c %*% b1.1)^2)
cat("Training Error (BFGS):", train_error, "\n")

# === Coordinate Descent for Lasso ===
# 軟閾值函數
soft_threshold <- function(x, ld) {
  ifelse(x > ld, x - ld, ifelse(x < -ld, x + ld, 0))
}

# 坐標下降法實現 Lasso
lasso_cd <- function(X, y, ld, max_iter = 100, tol = 1e-6) {
  p <- ncol(X)
  b <- rep(0, p)
  for (iter in 1:max_iter) {
    b_old <- b
    for (j in 1:p) {
      r_j <- y - X[, -j] %*% b[-j]
      b[j] <- soft_threshold(sum(X[, j] * r_j), ld) / sum(X[, j]^2)
    }
    if (max(abs(b - b_old)) < tol) break
  }
  return(b)
}

# 坐標下降法訓練 Lasso 模型
b1_cd <- lasso_cd(X1.c, y1.c, ld)
b0_cd <- mean(y1 - X1 %*% b1_cd)

# 訓練誤差
train_error_cd <- mean((y1.c - X1.c %*% b1_cd)^2)
cat("Training Error (CD):", train_error_cd, "\n")

# === 可視化結果 ===
plot(b1.true, ylim = c(-3, 3), cex = 2, pch = 20, col = "red", main = "True vs Estimated Coefficients")
points(b1_cd, cex = 1, pch = 20, col = "blue")
legend("topright", legend = c("True", "CD"), col = c("red", "blue"), pch = 20)
