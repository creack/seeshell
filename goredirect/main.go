package main

import (
	"context"
	"fmt"
	"os"
	"os/exec"
)

func test(ctx context.Context) error {
	os.Pipe()
	outFile, err := os.Create("out.txt")
	if err != nil {
		return fmt.Errorf("os.Create: %w", err)
	}
	defer func() { _ = outFile.Close() }()

	cmd := exec.Command("ls")
	cmd.Stdout = outFile
	return cmd.Run()
}

func main() {
	if err := test(context.Background()); err != nil {
		println("Fail:", err.Error())
		return
	}
	println("success")
}
