--- files/before/verifier.c	2020-12-03 05:28:07.761032290 +0530
+++ files/after/verifier.c	2020-12-03 05:28:08.813200513 +0530
@@ -2408,7 +2408,13 @@
 			 * remember the value we stored into this reg
 			 */
 			regs[insn->dst_reg].type = SCALAR_VALUE;
-			__mark_reg_known(regs + insn->dst_reg, insn->imm);
+			if (BPF_CLASS(insn->code) == BPF_ALU64) {
+				__mark_reg_known(regs + insn->dst_reg,
+						 insn->imm);
+			} else {
+				__mark_reg_known(regs + insn->dst_reg,
+						 (u32)insn->imm);
+			}
 		}
 
 	} else if (opcode > BPF_END) {
