package main

import (
	"fmt"
	"math/rand"
	"time"
)

func main() {
	rand.NewSource(time.Now().UnixNano())

	N := rand.Intn(61) + 120

	operators := []string{"+", "-", "*", "/"}

	for i := 0; i < N; i++ {
		X := rand.Intn(9) + 1
		Y := rand.Intn(9) + 1

		O := operators[rand.Intn(len(operators))]

		fmt.Printf("%d %s %d\n", X, O, Y)

		time.Sleep(1 * time.Second)
	}
}
