module ledboardfifo(
    input clk,                // Clock signal
    input rst,              // Active-low reset
    input write_en,           // Write enable
    input read_en,            // Read enable
    input [7:0] data_in,     // Data input (32 bits)
    output [7:0] data_out, // Data output (32 bits)
    output full,              // FIFO full flag
    output empty            // FIFO empty flag
);
    
    // FIFO parameters
    parameter WIDTH = 8;     // Data width (8 bits)
    parameter DEPTH = 66;     // FIFO depth (66 entries)
    localparam ADDR_WIDTH = 7;
    
    // Internal FIFO memory
    reg [WIDTH-1:0] fifo_mem [0:DEPTH-1];
    
    // Write and read pointers
    reg [ADDR_WIDTH-1:0] write_ptr;
    reg [ADDR_WIDTH-1:0] read_ptr;
    
    // Data out valid flag
    reg [WIDTH-1:0] data_out_reg; 
    
    // Write operation
    integer i;
    always @(posedge clk or posedge rst) begin
        if (rst) 
        begin
            write_ptr <= 7'b0;
            // Reset the FIFO memory to all 0s
            for (i = 0; i < DEPTH; i = i + 1) 
            begin
                fifo_mem[i] <= 8'b0;
            end
        end 
        else if (write_en) 
        begin
            if (write_ptr == 7'b0) begin
                // Allow first word to fall through without checking 'full'
                fifo_mem[write_ptr] <= data_in; // Write data to FIFO
                // No pointer increment for first write
            end else if (!full) begin
                // For subsequent writes, increment the pointer as usual
                fifo_mem[write_ptr] <= data_in; // Write data to FIFO
            end
            //write_ptr <= (write_ptr + 1) % DEPTH; // Normal pointer increment
            if (write_ptr<7'd65) write_ptr <= write_ptr + 1'b1;     
            else write_ptr <= 7'b0;
          
        end
    end

    //first word all through
    always @(posedge clk or posedge rst) begin
        if (rst) begin
            data_out_reg <= 8'b0;
        end else begin
            // Always output the first word in the FIFO
            data_out_reg <= fifo_mem[read_ptr]; // Read data from FIFO
        end
    end
    
    // Read operation
    always @(posedge clk or posedge rst) begin
        if (rst) 
        begin
            read_ptr <= 7'b0;
            
        end 
        else if (read_en && !empty) 
        begin
            if (read_ptr<7'd65) read_ptr <= read_ptr + 1'b1;
            else read_ptr <= 7'b0;
        end 
    end

    reg empty_reg;
    always @(posedge clk or posedge rst) 
    begin
        if (rst) empty_reg <= 1'b1;
        else if(write_ptr==read_ptr) empty_reg <= 1'b1;
        else empty_reg <= 1'b0;
    end
    assign empty = empty_reg;

    //Full condition logic
    wire [6:0] next_write_ptr;
    assign next_write_ptr = (write_ptr == 7'd65) ? 0 : write_ptr + 1'b1;

    reg full_reg;
    always @(posedge clk or posedge rst) 
    begin
        if (rst) full_reg <= 1'b1;
        else if(next_write_ptr==read_ptr) full_reg <= 1'b1;
        else full_reg <= 1'b0;
    end
    assign full = full_reg;

    assign data_out = data_out_reg;

endmodule