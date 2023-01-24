void send_msg(int arg0) {
    var_4 = arg0;
    var_68 = ebx;
    edi = dup(0x0);
    esp = (esp - 0x1c) + 0x10;
    if (edi == 0xffffffff) {
            printf("ERROR: dup(0) error\n");
            exit(0x8);
    }
    else {
            esp = (esp - 0x10) + 0x10;
            if (close(0x0) == 0xffffffff) {
                    printf("ERROR: close error\n");
                    exit(0x8);
            }
            else {
                    esi = tmpfile();
                    if (esi == 0x0) {
                            printf("ERROR: tmpfile error\n");
                            exit(0x8);
                    }
                    else {
                            fprintf(esi, "Subject: Bomb notification\n");
                            fprintf(esi, 0x8049c01);
                            eax = cuserid(0x0);
                            esp = ((esp - 0x20) + 0x20 - 0x10) + 0x10;
                            if (eax == 0x0) {
                                    var_50 = 0x6f626f6e;
                                    *(int16_t *)(&var_50 + 0x4) = 0x7964;
                                    *(int8_t *)(&var_50 + 0x6) = 0x0;
                            }
                            else {
                                    eax = strcpy(&var_50, eax);
                                    esp = (esp - 0x10) + 0x10;
                            }
                            esp = esp - 0x8;
                            eax = "exploded";
                            if (var_4 != 0x0) {
                                    eax = "defused";
                            }
                            stack[-116] = *num_input_strings;
                            stack[-120] = eax;
                            eax = *0x804ae00;
                            eax = fprintf(esi, "bomb-header:%s:%d:%s:%s:%d\n", "generic", eax, &var_50, stack[-120], stack[-116]);
                            edx = 0x0;
                            esp = (esp - 0x18) + 0x20;
                            if (edx < *num_input_strings) {
                                    do {
                                            stack[-116] = (edx + edx * 0x4 << 0x4) + 0x804b680;
                                            stack[-120] = edx + 0x1;
                                            eax = *0x804ae00;
                                            eax = fprintf(esi, "bomb-string:%s:%d:%s:%d:%s\n", "generic", eax, &var_50, stack[-120], stack[-116]);
                                            esp = (esp - 0x20) + 0x20;
                                            edx = edx + 0x1;
                                    } while (edx < *num_input_strings);
                            }
                            rewind(esi);
                            sprintf(0x804bcc0, "%s %s@%s", "/usr/sbin/sendmail -bm", 0x8049c53, "bluegill.cmcl.cs.cmu.edu");
                            esp = ((esp - 0x30) + 0x30 - 0x10) + 0x10;
                            if (system(0x804bcc0) != 0x0) {
                                    printf("ERROR: notification error\n");
                                    exit(0x8);
                            }
                            else {
                                    esp = (esp - 0x10) + 0x10;
                                    if (fclose(esi) != 0x0) {
                                            printf("ERROR: fclose(tmp) error\n");
                                            exit(0x8);
                                    }
                                    else {
                                            esp = (esp - 0x10) + 0x10;
                                            if (dup(edi) != 0x0) {
                                                    printf("ERROR: dup(tmpstdin) error\n");
                                                    exit(0x8);
                                            }
                                            else {
                                                    esp = (esp - 0x10) + 0x10;
                                                    if (close(edi) != 0x0) {
                                                            printf("ERROR: close(tmpstdin)\n");
                                                            exit(0x8);
                                                    }
                                            }
                                    }
                            }
                    }
            }
    }
    return;
}













void phase_defused() {
    esp = esp - 0x4;
    var_68 = ebx;
    if (*num_input_strings == 0x6) {
            ebx = &var_50;
            eax = sscanf(0x804b770, "%d %s", &var_54, ebx);
            esp = (esp - 0x10) + 0x10;
            if (eax == 0x2) {
                    esp = (esp - 0x10) + 0x10;
                    if (strings_not_equal(ebx, "austinpowers") == 0x0) {
                            printf("Curses, you've found the secret phase!\n");
                            printf("But finding it and solving it are quite different...\n");
                            esp = (esp - 0x20) + 0x20;
                            secret_phase();
                    }
            }
            printf("Congratulations! You've defused the bomb!\n");
    }
    return;
}










int read_line() {
    esp = esp - 0x4;
    var_18 = edi;
    if (skip() == 0x0) {
            if (*infile != *stdin@@GLIBC_2.0) {
                    esp = (esp - 0x10) + 0x10;
                    if (getenv("GRADE_BOMB") != 0x0) {
                            eax = exit(0x0);
                    }
                    else {
                            *infile = *stdin@@GLIBC_2.0;
                            if (skip() == 0x0) {
                                    printf("Error: Premature EOF on stdin\n");
                                    eax = explode_bomb();
                            }
                            else {
                                    eax = *num_input_strings;
                                    edi = (eax + eax * 0x4 << 0x4) + 0x804b680;
                                    eax = 0x0;
                                    asm { cld };
                                    ecx = 0xffffffff;
                                    asm { repne scasb al, byte [edi] };
                                    edi = !ecx - 0x1;
                                    if (edi == 0x4f) {
                                            printf("Error: Input line too long\n");
                                            eax = explode_bomb();
                                    }
                                    else {
                                            eax = *num_input_strings;
                                            *(int8_t *)(edi + (eax + eax * 0x4 << 0x4) + 0x804b67f) = 0x0;
                                            eax = (eax + eax * 0x4 << 0x4) + 0x804b680;
                                            *num_input_strings = *num_input_strings + 0x1;
                                    }
                            }
                    }
            }
            else {
                    printf("Error: Premature EOF on stdin\n");
                    eax = explode_bomb();
            }
    }
    else {
            eax = *num_input_strings;
            edi = (eax + eax * 0x4 << 0x4) + 0x804b680;
            eax = 0x0;
            asm { cld };
            ecx = 0xffffffff;
            asm { repne scasb al, byte [edi] };
            edi = !ecx - 0x1;
            if (edi == 0x4f) {
                    printf("Error: Input line too long\n");
                    eax = explode_bomb();
            }
            else {
                    eax = *num_input_strings;
                    *(int8_t *)(edi + (eax + eax * 0x4 << 0x4) + 0x804b67f) = 0x0;
                    eax = (eax + eax * 0x4 << 0x4) + 0x804b680;
                    *num_input_strings = *num_input_strings + 0x1;
            }
    }
    return eax;
}












void open_clientfd(int arg0, int arg1) {
    var_8 = arg1;
    var_4 = arg0;
    var_28 = ebx;
    esi = socket(0x2, 0x1, 0x0);
    esp = (esp - 0x18) + 0x10;
    if (esi < 0x0) {
            printf("Bad host (1).\n");
            exit(0x8);
    }
    else {
            ebx = gethostbyname(var_4);
            esp = (esp - 0x10) + 0x10;
            if (ebx == 0x0) {
                    printf("Bad host (2).\n");
                    exit(0x8);
            }
            else {
                    bzero(&var_10, 0x10);
                    var_10 = 0x2;
                    bcopy(**(ebx + 0x10), &var_C, *(ebx + 0xc));
                    eax = connect(esi, &var_10, 0x10);
                    esp = ((esp - 0x20) + 0x20 - 0x10) + 0x10;
                    if (eax < 0x0) {
                            printf("Bad host (3).\n");
                            exit(0x8);
                    }
            }
    }
    return;
}














int strings_not_equal(int arg0, int arg1) {
    var_18 = ebx;
    esi = arg0;
    edi = arg1;
    if (string_length(esi) == string_length(edi)) goto loc_8049060;

loc_8049057:
    eax = 0x1;
    return eax;

loc_8049060:
    edx = esi;
    ecx = edi;
    if (*(int8_t *)edx == 0x0) goto loc_804907d;

loc_8049070:
    if (*(int8_t *)edx != *(int8_t *)ecx) goto loc_8049057;

loc_8049076:
    edx = edx + 0x1;
    ecx = ecx + 0x1;
    if (*(int8_t *)edx != 0x0) goto loc_8049070;

loc_804907d:
    eax = 0x0;
    return eax;
}














int secret_phase() {
    var_18 = ebx;
    esp = (esp - 0x14) + 0x10;
    ebx = __strtol_internal(read_line(), 0x0, 0xa, 0x0);
    if (ebx > 0x3e9) {
            eax = explode_bomb();
    }
    else {
            esp = (esp - 0x10) + 0x10;
            if (fun7(0x804b320, ebx) != 0x7) {
                    eax = explode_bomb();
            }
            else {
                    printf("Wow! You've defused the secret stage!\n");
                    eax = phase_defused();
            }
    }
    return eax;
}












int fun7(int arg0, int arg1) {
    edx = arg0;
    eax = arg1;
    if (edx == 0x0) {
            eax = 0xffffffff;
    }
    else {
            if (eax < *edx) {
                    stack[-24] = eax;
                    eax = *(edx + 0x4);
                    eax = fun7(eax, stack[-24]);
                    eax = eax + eax;
            }
            else {
                    if (eax != *edx) {
                            stack[-24] = eax;
                            eax = *(edx + 0x8);
                            eax = fun7(eax, stack[-24]);
                            eax = 0x1 + eax + eax;
                    }
                    else {
                            eax = 0x0;
                    }
            }
    }
    return eax;
}









int phase_6(int arg0) {
    var_58 = ebx;
    stack[-104] = &var_18;
    read_six_numbers(arg0, stack[-104]);
    edi = 0x0;
    esi = esi;
    goto loc_8048dc0;

loc_8048dc0:
    if (*(&var_18 + edi * 0x4) <= 0x6) goto loc_8048dd1;

loc_8048dcc:
    eax = explode_bomb();
    return eax;

.l1:
    return eax;

loc_8048dd1:
    ebx = edi + 0x1;
    if (ebx > 0x5) goto loc_8048dfc;

loc_8048dd9:
    var_38 = edi * 0x4;
    esi = &var_18;
    goto loc_8048de6;

loc_8048de6:
    if (*(var_38 + esi) != *(esi + ebx * 0x4)) goto loc_8048df6;

loc_8048df1:
    eax = explode_bomb();
    return eax;

loc_8048df6:
    ebx = ebx + 0x1;
    if (ebx <= 0x5) goto loc_8048de6;

loc_8048dfc:
    edi = edi + 0x1;
    if (edi <= 0x5) goto loc_8048dc0;

loc_8048e02:
    edi = 0x0;
    ecx = &var_18;
    var_3C = &var_30;
    do {
            esi = 0x804b26c;
            edx = edi * 0x4;
            if (0x1 < *(edi * 0x4 + ecx)) {
                    ebx = 0x1;
                    eax = *(edx + ecx);
                    esi = 0x804b26c;
                    do {
                            esi = *(esi + 0x8);
                            ebx = ebx + 0x1;
                    } while (ebx < eax);
            }
            *(var_3C + edi * 0x4) = esi;
            edi = edi + 0x1;
    } while (edi <= 0x5);
    esi = var_30;
    var_34 = esi;
    edi = 0x1;
    edx = &var_30;
    do {
            eax = *(edx + edi * 0x4);
            *(esi + 0x8) = eax;
            esi = eax;
            edi = edi + 0x1;
    } while (edi <= 0x5);
    *(esi + 0x8) = 0x0;
    edi = 0x0;
    esi = var_34;
    goto loc_8048e70;

loc_8048e70:
    edx = *(esi + 0x8);
    eax = *esi;
    if (eax >= *edx) goto loc_8048e7e;

loc_8048e79:
    eax = explode_bomb();
    return eax;

loc_8048e7e:
    esi = *(esi + 0x8);
    edi = edi + 0x1;
    if (edi <= 0x4) goto loc_8048e70;
}








void phase_5(int arg0) {
    var_18 = ebx;
    ebx = arg0;
    esp = (esp - 0x18) + 0x10;
    if (string_length(ebx) != 0x6) {
            explode_bomb();
    }
    else {
            edx = 0x0;
            ecx = &var_8;
            do {
                    *(int8_t *)(edx + ecx) = *(int8_t *)(sign_extend_32(*(int8_t *)(edx + ebx) & 0xf) + 0x804b220);
                    edx = edx + 0x1;
            } while (edx <= 0x5);
            eax = strings_not_equal(&var_8, "giants");
            if (eax != 0x0) {
                    explode_bomb();
            }
    }
    return;
}











int phase_4(int arg0) {
    esp = (esp - 0x10) + 0x10;
    if (sscanf(arg0, 0x8049808) == 0x1 && var_4 > 0x0) {
            eax = func4(var_4);
            if (eax != 0x37) {
                    eax = explode_bomb();
            }
    }
    else {
            eax = explode_bomb();
    }
    return eax;
}







int phase_3(int arg0) {
    var_18 = ebx;
    eax = sscanf(arg0, "%d %c %d", &var_C, &var_5, &var_4);
    if (eax <= 0x2) {
            eax = explode_bomb();
    }
    else {
            if (var_C <= 0x7) {
                    eax = var_C;
                    switch (*(eax * 0x4 + 0x80497e8)) {
                        case 0:
                            ebx = 0x71;
                            if (var_4 != 0x309) {
                                    eax = explode_bomb();
                            }
                            else {
                                    if (ebx != var_5) {
                                            eax = explode_bomb();
                                    }
                            }
                            break;
                        case 1:
                            ebx = 0x62;
                            if (var_4 != 0xd6) {
                                    eax = explode_bomb();
                            }
                            else {
                                    if (ebx != var_5) {
                                            eax = explode_bomb();
                                    }
                            }
                            break;
                        case 2:
                            ebx = 0x62;
                            if (var_4 != 0x2f3) {
                                    eax = explode_bomb();
                            }
                            else {
                                    if (ebx != var_5) {
                                            eax = explode_bomb();
                                    }
                            }
                            break;
                        case 3:
                            ebx = 0x6b;
                            if (var_4 != 0xfb) {
                                    eax = explode_bomb();
                            }
                            else {
                                    if (ebx != var_5) {
                                            eax = explode_bomb();
                                    }
                            }
                            break;
                        case 4:
                            ebx = 0x6f;
                            if (var_4 != 0xa0) {
                                    eax = explode_bomb();
                            }
                            else {
                                    if (ebx != var_5) {
                                            eax = explode_bomb();
                                    }
                            }
                            break;
                        case 5:
                            ebx = 0x74;
                            if (var_4 != 0x1ca) {
                                    eax = explode_bomb();
                            }
                            else {
                                    if (ebx != var_5) {
                                            eax = explode_bomb();
                                    }
                            }
                            break;
                        case 6:
                            ebx = 0x76;
                            if (var_4 != 0x30c) {
                                    eax = explode_bomb();
                            }
                            else {
                                    if (ebx != var_5) {
                                            eax = explode_bomb();
                                    }
                            }
                            break;
                        case 7:
                            ebx = 0x62;
                            if (var_4 != 0x20c) {
                                    eax = explode_bomb();
                            }
                            else {
                                    if (ebx != var_5) {
                                            eax = explode_bomb();
                                    }
                            }
                            break;
                    }
            }
            else {
                    eax = explode_bomb();
            }
    }
    return eax;
}














int phase_3(int arg0) {
    var_18 = ebx;
    eax = sscanf(arg0, "%d %c %d", &var_C, &var_5, &var_4);
    if (eax <= 0x2) {
            eax = explode_bomb();
    }
    else {
            if (var_C <= 0x7) {
                    eax = var_C;
                    switch (*(eax * 0x4 + 0x80497e8)) {
                        case 0:
                            ebx = 0x71;
                            if (var_4 != 0x309) {
                                    eax = explode_bomb();
                            }
                            else {
                                    if (ebx != var_5) {
                                            eax = explode_bomb();
                                    }
                            }
                            break;
                        case 1:
                            ebx = 0x62;
                            if (var_4 != 0xd6) {
                                    eax = explode_bomb();
                            }
                            else {
                                    if (ebx != var_5) {
                                            eax = explode_bomb();
                                    }
                            }
                            break;
                        case 2:
                            ebx = 0x62;
                            if (var_4 != 0x2f3) {
                                    eax = explode_bomb();
                            }
                            else {
                                    if (ebx != var_5) {
                                            eax = explode_bomb();
                                    }
                            }
                            break;
                        case 3:
                            ebx = 0x6b;
                            if (var_4 != 0xfb) {
                                    eax = explode_bomb();
                            }
                            else {
                                    if (ebx != var_5) {
                                            eax = explode_bomb();
                                    }
                            }
                            break;
                        case 4:
                            ebx = 0x6f;
                            if (var_4 != 0xa0) {
                                    eax = explode_bomb();
                            }
                            else {
                                    if (ebx != var_5) {
                                            eax = explode_bomb();
                                    }
                            }
                            break;
                        case 5:
                            ebx = 0x74;
                            if (var_4 != 0x1ca) {
                                    eax = explode_bomb();
                            }
                            else {
                                    if (ebx != var_5) {
                                            eax = explode_bomb();
                                    }
                            }
                            break;
                        case 6:
                            ebx = 0x76;
                            if (var_4 != 0x30c) {
                                    eax = explode_bomb();
                            }
                            else {
                                    if (ebx != var_5) {
                                            eax = explode_bomb();
                                    }
                            }
                            break;
                        case 7:
                            ebx = 0x62;
                            if (var_4 != 0x20c) {
                                    eax = explode_bomb();
                            }
                            else {
                                    if (ebx != var_5) {
                                            eax = explode_bomb();
                                    }
                            }
                            break;
                    }
            }
            else {
                    eax = explode_bomb();
            }
    }
    return eax;
}










int phase_2(int arg0) {
    var_28 = ebx;
    eax = read_six_numbers(arg0, &var_18);
    if (var_18 == 0x1) goto loc_8048b6e;

loc_8048b69:
    eax = explode_bomb();
    return eax;

.l1:
    return eax;

loc_8048b6e:
    ebx = 0x1;
    esi = &var_18;
    goto loc_8048b76;

loc_8048b76:
    eax = (ebx + 0x1) * *(esi + (ebx * 0x4 - 0x4));
    if (*(esi + ebx * 0x4) == eax) goto loc_8048b88;

loc_8048b83:
    eax = explode_bomb();
    return eax;

loc_8048b88:
    ebx = ebx + 0x1;
    if (ebx <= 0x5) goto loc_8048b76;
}












int main(int arg0, int arg1) {
    esp = esp - 0x4;
    var_18 = ebx;
    eax = arg0;
    ebx = arg1;
    if (eax == 0x1) {
            *infile = *stdin@@GLIBC_2.0;
            initialize_bomb();
            printf("Welcome this is my little bomb !!!! You have 6 stages with\n");
            printf("only one life good luck !! Have a nice day!\n");
            phase_1(read_line());
            phase_defused();
            printf("Phase 1 defused. How about the next one?\n");
            phase_2(read_line());
            phase_defused();
            printf("That's number 2.  Keep going!\n");
            phase_3(read_line());
            phase_defused();
            printf("Halfway there!\n");
            phase_4(read_line());
            phase_defused();
            printf("So you got that one.  Try this one.\n");
            phase_5(read_line());
            phase_defused();
            printf("Good work!  On to the next...\n");
            phase_6(read_line());
            phase_defused();
            eax = 0x0;
    }
    else {
            if (eax == 0x2) {
                    eax = *(ebx + 0x4);
                    eax = fopen(eax, 0x8049620);
                    *infile = eax;
                    esp = (esp - 0x10) + 0x10;
                    if (eax == 0x0) {
                            printf("%s: Error: Couldn't open %s\n", *ebx, *(ebx + 0x4));
                            eax = exit(0x8);
                    }
                    else {
                            initialize_bomb();
                            printf("Welcome this is my little bomb !!!! You have 6 stages with\n");
                            printf("only one life good luck !! Have a nice day!\n");
                            phase_1(read_line());
                            phase_defused();
                            printf("Phase 1 defused. How about the next one?\n");
                            phase_2(read_line());
                            phase_defused();
                            printf("That's number 2.  Keep going!\n");
                            phase_3(read_line());
                            phase_defused();
                            printf("Halfway there!\n");
                            phase_4(read_line());
                            phase_defused();
                            printf("So you got that one.  Try this one.\n");
                            phase_5(read_line());
                            phase_defused();
                            printf("Good work!  On to the next...\n");
                            phase_6(read_line());
                            phase_defused();
                            eax = 0x0;
                    }
            }
            else {
                    printf("Usage: %s [<input_file>]\n", *ebx);
                    eax = exit(0x8);
            }
    }
    return eax;
}


void __do_global_dtors_aux() {
    if (*completed.4 == 0x0) {
            while (**p.3 != 0x0) {
                    eax = *p.3;
                    *p.3 = eax + 0x4;
                    (*eax)();
            }
            if (0x80487c0 != 0x0) {
                    __deregister_frame_info(0x804b484);
            }
            *completed.4 = 0x1;
    }
    return;
}
