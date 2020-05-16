%assign limit 29

is_prime: dd 0x4  ; 32bits

%assign n 3
%rep limit
  %assign current 1
  %assign i 1
    %rep n/2
      %assign i i+1
      %if n % i = 0
        %assign current 0
        %exitrep
      %endif
    %endrep
  add [is_prime], current
  shl [is_prime]
  %assign n n+1
%endrep



