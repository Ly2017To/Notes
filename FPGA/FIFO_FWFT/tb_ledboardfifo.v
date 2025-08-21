module tb_ledboardfifo;

    // Parameters
    parameter WIDTH = 8;
    parameter DEPTH = 64;

    // Signals
    reg clk;
    reg rst;
    reg write_en;
    reg read_en;
    reg [7:0] data_in;
    wire [7:0] data_out;
    wire full;
    wire empty;

    // Instantiate the FIFO module
    ledboardfifo #(
        .WIDTH(WIDTH),
        .DEPTH(DEPTH)
    ) uut (
        .clk(clk),
        .rst(rst),
        .write_en(write_en),
        .read_en(read_en),
        .data_in(data_in),
        .data_out(data_out),
        .full(full),
        .empty(empty)
    );

    // Clock generation
    always begin
        #5 clk = ~clk;  // Clock period of 10 time units
    end

    // Test procedure
    initial begin
        // Initialize signals
        clk = 0;
        rst = 0;
        write_en = 0;
        read_en = 0;
        data_in = 0;

        // Apply reset
        $display("Applying reset...");
        rst = 1;  // Assert reset
        #10;
        rst = 0;  // Deassert reset
        #10;

        // Write some data
        $display("Writing data...");
        write_en = 1;
        data_in = 8'h01;
        #10;
        data_in = 8'h02;
        #10;
        data_in = 8'h03;
        #10;
        write_en = 0;

        // Read data
        $display("Reading data...");
        read_en = 1;
        #10; // Read 1st item
        $display("Data Out: %h", data_out);
        #10; // Read 2nd item
        $display("Data Out: %h", data_out);
        #10; // Read 3rd item
        $display("Data Out: %h", data_out);
        read_en = 0;

        // Check if FIFO is empty after reads
        $display("FIFO Empty Flag: %b", empty);

        // Write more data
        $display("Writing more data...");
        write_en = 1;
        data_in = 8'hAA;
        #10;
        data_in = 8'hBB;
        #10;
        data_in = 8'hCC;
        #10;
        write_en = 0;

        // Check full flag after writing
        $display("FIFO Full Flag: %b", full);

        // Perform read operation again
        $display("Reading data again...");
        read_en = 1;
        #10; // Read 1st item after new writes
        $display("Data Out: %h", data_out);
        #10; // Read 2nd item
        $display("Data Out: %h", data_out);
        #10; // Read 3rd item
        $display("Data Out: %h", data_out);
        read_en = 0;

        // Check empty flag after read operations
        $display("FIFO Empty Flag: %b", empty);

        // Test FIFO full condition
        $display("Testing FIFO full condition...");
        write_en = 1;
        for (data_in = 8'h01; data_in <= 8'h42; data_in = data_in + 1) begin
            #10; // Wait 10ns before next write
            $display("Writing data: %h", data_in);
        end
        write_en = 0;
        $display("FIFO Full Flag: %b", full);

        // End simulation
        $stop;
    end

    // Do this in your test bench
    initial
    begin
        $dumpfile("tb_ledboardfifo.vcd");
        $dumpvars(0,tb_ledboardfifo);
    end

endmodule