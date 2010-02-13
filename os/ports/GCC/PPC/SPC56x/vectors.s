/*
    ChibiOS/RT - Copyright (C) 2006-2007 Giovanni Di Sirio.

    This file is part of ChibiOS/RT.

    ChibiOS/RT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS/RT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/**
 * @file    PPC/SPC56x/vectors.s
 * @brief   SPC56x vectors table.
 *
 * @addtogroup PPC_CORE
 * @{
 */
/** @cond never */

        /*
         * BAM info, SWT off, WTE off, VLE off.
         */
        .section    .bam
        .long       0x005A0000
        .long       _boot_address

        /*
         * Software vectors table. The vectors are accessed from the IVOR4
         * handler only. In order to declare an interrupt handler just create
         * a function withe the same name of a vector, the symbol will
         * override the weak symbol declared here.
         */
        .section    .vectors
        .align		2
        .globl      _vectors
_vectors:
        .word       vector0
        .word       vector1
        .word       vector2
        .word       vector3
        .word       vector4
        .word       vector5
        .word       vector6
        .word       vector7
        .word       vector8
        .word       vector9
        .word       vector10
        .word       vector11
        .word       vector12
        .word       vector13
        .word       vector14
        .word       vector15
        .word       vector16
        .word       vector17
        .word       vector18
        .word       vector19
        .word       vector20
        .word       vector21
        .word       vector22
        .word       vector23
        .word       vector24
        .word       vector25
        .word       vector26
        .word       vector27
        .word       vector28
        .word       vector29
        .word       vector30
        .word       vector31
        .word       vector32
        .word       vector33
        .word       vector34
        .word       vector35
        .word       vector36
        .word       vector37
        .word       vector38
        .word       vector39
        .word       vector40
        .word       vector41
        .word       vector42
        .word       vector43
        .word       vector44
        .word       vector45
        .word       vector46
        .word       vector47
        .word       vector48
        .word       vector49
        .word       vector50
        .word       vector51
        .word       vector52
        .word       vector53
        .word       vector54
        .word       vector55
        .word       vector56
        .word       vector57
        .word       vector58
        .word       vector59
        .word       vector60
        .word       vector61
        .word       vector62
        .word       vector63
        .word       vector64
        .word       vector65
        .word       vector66
        .word       vector67
        .word       vector68
        .word       vector69
        .word       vector70
        .word       vector71
        .word       vector72
        .word       vector73
        .word       vector74
        .word       vector75
        .word       vector76
        .word       vector77
        .word       vector78
        .word       vector79
        .word       vector80
        .word       vector81
        .word       vector82
        .word       vector83
        .word       vector84
        .word       vector85
        .word       vector86
        .word       vector87
        .word       vector88
        .word       vector89
        .word       vector90
        .word       vector91
        .word       vector92
        .word       vector93
        .word       vector94
        .word       vector95
        .word       vector96
        .word       vector97
        .word       vector98
        .word       vector99
        .word       vector100
        .word       vector101
        .word       vector102
        .word       vector103
        .word       vector104
        .word       vector105
        .word       vector106
        .word       vector107
        .word       vector108
        .word       vector109
        .word       vector110
        .word       vector111
        .word       vector112
        .word       vector113
        .word       vector114
        .word       vector115
        .word       vector116
        .word       vector117
        .word       vector118
        .word       vector119
        .word       vector120
        .word       vector121
        .word       vector122
        .word       vector123
        .word       vector124
        .word       vector125
        .word       vector126
        .word       vector127
        .word       vector128
        .word       vector129
        .word       vector130
        .word       vector131
        .word       vector132
        .word       vector133
        .word       vector134
        .word       vector135
        .word       vector136
        .word       vector137
        .word       vector138
        .word       vector139
        .word       vector140
        .word       vector141
        .word       vector142
        .word       vector143
        .word       vector144
        .word       vector145
        .word       vector146
        .word       vector147
        .word       vector148
        .word       vector149
        .word       vector150
        .word       vector151
        .word       vector152
        .word       vector153
        .word       vector154
        .word       vector155
        .word       vector156
        .word       vector157
        .word       vector158
        .word       vector159
        .word       vector160
        .word       vector161
        .word       vector162
        .word       vector163
        .word       vector164
        .word       vector165
        .word       vector166
        .word       vector167
        .word       vector168
        .word       vector169
        .word       vector170
        .word       vector171
        .word       vector172
        .word       vector173
        .word       vector174
        .word       vector175
        .word       vector176
        .word       vector177
        .word       vector178
        .word       vector179
        .word       vector180
        .word       vector181
        .word       vector182
        .word       vector183
        .word       vector184
        .word       vector185
        .word       vector186
        .word       vector187
        .word       vector188
        .word       vector189
        .word       vector190
        .word       vector191
        .word       vector192
        .word       vector193
        .word       vector194
        .word       vector195
        .word       vector196
        .word       vector197
        .word       vector198
        .word       vector199
        .word       vector200
        .word       vector201
        .word       vector202
        .word       vector203
        .word       vector204
        .word       vector205
        .word       vector206
        .word       vector207
        .word       vector208
        .word       vector209
        .word       vector210
        .word       vector211
        .word       vector212
        .word       vector213
        .word       vector214
        .word       vector215
        .word       vector216
        .word       vector217
        .word       vector218
        .word       vector219
        .word       vector220
        .word       vector221
        .word       vector222
        .word       vector223
        .word       vector224
        .word       vector225
        .word       vector226
        .word       vector227
        .word       vector228
        .word       vector229
        .word       vector230
        .word       vector231
        .word       vector232
        .word       vector233
        .word       vector234
        .word       vector235
        .word       vector236
        .word       vector237
        .word       vector238
        .word       vector239
        .word       vector240
        .word       vector241
        .word       vector242
        .word       vector243
        .word       vector244
        .word       vector245
        .word       vector246
        .word       vector247
        .word       vector248
        .word       vector249
        .word       vector250
        .word       vector251
        .word       vector252
        .word       vector253
        .word       vector254
        .word       vector255
        .word       vector256
        .word       vector257
        .word       vector258
        .word       vector259
        .word       vector260
        .word       vector261
        .word       vector262
        .word       vector263
        .word       vector264
        .word       vector265
        .word       vector266
        .word       vector267
        .word       vector268
        .word       vector269
        .word       vector270
        .word       vector271
        .word       vector272
        .word       vector273
        .word       vector274
        .word       vector275
        .word       vector276
        .word       vector277
        .word       vector278
        .word       vector279
        .word       vector280
        .word       vector281
        .word       vector282
        .word       vector283
        .word       vector284
        .word       vector285
        .word       vector286
        .word       vector287
        .word       vector288
        .word       vector289
        .word       vector290
        .word       vector291
        .word       vector292
        .word       vector293
        .word       vector294
        .word       vector295
        .word       vector296
        .word       vector297
        .word       vector298
        .word       vector299
        .word       vector300
        .word       vector301
        .word       vector302
        .word       vector303
        .word       vector304
        .word       vector305
        .word       vector306
        .word       vector307
        .word       vector308
        .word       vector309
        .word       vector310
        .word       vector311
        .word       vector312
        .word       vector313
        .word       vector314
        .word       vector315
        .word       vector316
        .word       vector317
        .word       vector318
        .word       vector319
        .word       vector320
        .word       vector321
        .word       vector322
        .word       vector323
        .word       vector324
        .word       vector325
        .word       vector326
        .word       vector327
        .word       vector328
        .word       vector329
        .word       vector330
        .word       vector331
        .word       vector332
        .word       vector333
        .word       vector334
        .word       vector335
        .word       vector336
        .word       vector337
        .word       vector338
        .word       vector339
        .word       vector340
        .word       vector341
        .word       vector342
        .word       vector343
        .word       vector344
        .word       vector345
        .word       vector346
        .word       vector347
        .word       vector348
        .word       vector349
        .word       vector350
        .word       vector351
        .word       vector352
        .word       vector353
        .word       vector354
        .word       vector355
        .word       vector356
        .word       vector357
        .word       vector358
        .word       vector359

        .text
        .align      2
        .weak       vector0
vector0:

        .weak       vector1
vector1:

        .weak       vector2
vector2:

        .weak       vector3
vector3:

        .weak       vector4
vector4:

        .weak       vector5
vector5:

        .weak       vector6
vector6:

        .weak       vector7
vector7:

        .weak       vector8
vector8:

        .weak       vector9
vector9:

        .weak       vector10
vector10:

        .weak       vector11
vector11:

        .weak       vector12
vector12:

        .weak       vector13
vector13:

        .weak       vector14
vector14:

        .weak       vector15
vector15:

        .weak       vector16
vector16:

        .weak       vector17
vector17:

        .weak       vector18
vector18:

        .weak       vector19
vector19:

        .weak       vector20
vector20:

        .weak       vector21
vector21:

        .weak       vector22
vector22:

        .weak       vector23
vector23:

        .weak       vector24
vector24:

        .weak       vector25
vector25:

        .weak       vector26
vector26:

        .weak       vector27
vector27:

        .weak       vector28
vector28:

        .weak       vector29
vector29:

        .weak       vector30
vector30:

        .weak       vector31
vector31:

        .weak       vector32
vector32:

        .weak       vector33
vector33:

        .weak       vector34
vector34:

        .weak       vector35
vector35:

        .weak       vector36
vector36:

        .weak       vector37
vector37:

        .weak       vector38
vector38:

        .weak       vector39
vector39:

        .weak       vector40
vector40:

        .weak       vector41
vector41:

        .weak       vector42
vector42:

        .weak       vector43
vector43:

        .weak       vector44
vector44:

        .weak       vector45
vector45:

        .weak       vector46
vector46:

        .weak       vector47
vector47:

        .weak       vector48
vector48:

        .weak       vector49
vector49:

        .weak       vector50
vector50:

        .weak       vector51
vector51:

        .weak       vector52
vector52:

        .weak       vector53
vector53:

        .weak       vector54
vector54:

        .weak       vector55
vector55:

        .weak       vector56
vector56:

        .weak       vector57
vector57:

        .weak       vector58
vector58:

        .weak       vector59
vector59:

        .weak       vector60
vector60:

        .weak       vector61
vector61:

        .weak       vector62
vector62:

        .weak       vector63
vector63:

        .weak       vector64
vector64:

        .weak       vector65
vector65:

        .weak       vector66
vector66:

        .weak       vector67
vector67:

        .weak       vector68
vector68:

        .weak       vector69
vector69:

        .weak       vector70
vector70:

        .weak       vector71
vector71:

        .weak       vector72
vector72:

        .weak       vector73
vector73:

        .weak       vector74
vector74:

        .weak       vector75
vector75:

        .weak       vector76
vector76:

        .weak       vector77
vector77:

        .weak       vector78
vector78:

        .weak       vector79
vector79:

        .weak       vector80
vector80:

        .weak       vector81
vector81:

        .weak       vector82
vector82:

        .weak       vector83
vector83:

        .weak       vector84
vector84:

        .weak       vector85
vector85:

        .weak       vector86
vector86:

        .weak       vector87
vector87:

        .weak       vector88
vector88:

        .weak       vector89
vector89:

        .weak       vector90
vector90:

        .weak       vector91
vector91:

        .weak       vector92
vector92:

        .weak       vector93
vector93:

        .weak       vector94
vector94:

        .weak       vector95
vector95:

        .weak       vector96
vector96:

        .weak       vector97
vector97:

        .weak       vector98
vector98:

        .weak       vector99
vector99:

        .weak       vector100
vector100:

        .weak       vector101
vector101:

        .weak       vector102
vector102:

        .weak       vector103
vector103:

        .weak       vector104
vector104:

        .weak       vector105
vector105:

        .weak       vector106
vector106:

        .weak       vector107
vector107:

        .weak       vector108
vector108:

        .weak       vector109
vector109:

        .weak       vector110
vector110:

        .weak       vector111
vector111:

        .weak       vector112
vector112:

        .weak       vector113
vector113:

        .weak       vector114
vector114:

        .weak       vector115
vector115:

        .weak       vector116
vector116:

        .weak       vector117
vector117:

        .weak       vector118
vector118:

        .weak       vector119
vector119:

        .weak       vector120
vector120:

        .weak       vector121
vector121:

        .weak       vector122
vector122:

        .weak       vector123
vector123:

        .weak       vector124
vector124:

        .weak       vector125
vector125:

        .weak       vector126
vector126:

        .weak       vector127
vector127:

        .weak       vector128
vector128:

        .weak       vector129
vector129:

        .weak       vector130
vector130:

        .weak       vector131
vector131:

        .weak       vector132
vector132:

        .weak       vector133
vector133:

        .weak       vector134
vector134:

        .weak       vector135
vector135:

        .weak       vector136
vector136:

        .weak       vector137
vector137:

        .weak       vector138
vector138:

        .weak       vector139
vector139:

        .weak       vector140
vector140:

        .weak       vector141
vector141:

        .weak       vector142
vector142:

        .weak       vector143
vector143:

        .weak       vector144
vector144:

        .weak       vector145
vector145:

        .weak       vector146
vector146:

        .weak       vector147
vector147:

        .weak       vector148
vector148:

        .weak       vector149
vector149:

        .weak       vector150
vector150:

        .weak       vector151
vector151:

        .weak       vector152
vector152:

        .weak       vector153
vector153:

        .weak       vector154
vector154:

        .weak       vector155
vector155:

        .weak       vector156
vector156:

        .weak       vector157
vector157:

        .weak       vector158
vector158:

        .weak       vector159
vector159:

        .weak       vector160
vector160:

        .weak       vector161
vector161:

        .weak       vector162
vector162:

        .weak       vector163
vector163:

        .weak       vector164
vector164:

        .weak       vector165
vector165:

        .weak       vector166
vector166:

        .weak       vector167
vector167:

        .weak       vector168
vector168:

        .weak       vector169
vector169:

        .weak       vector170
vector170:

        .weak       vector171
vector171:

        .weak       vector172
vector172:

        .weak       vector173
vector173:

        .weak       vector174
vector174:

        .weak       vector175
vector175:

        .weak       vector176
vector176:

        .weak       vector177
vector177:

        .weak       vector178
vector178:

        .weak       vector179
vector179:

        .weak       vector180
vector180:

        .weak       vector181
vector181:

        .weak       vector182
vector182:

        .weak       vector183
vector183:

        .weak       vector184
vector184:

        .weak       vector185
vector185:

        .weak       vector186
vector186:

        .weak       vector187
vector187:

        .weak       vector188
vector188:

        .weak       vector189
vector189:

        .weak       vector190
vector190:

        .weak       vector191
vector191:

        .weak       vector192
vector192:

        .weak       vector193
vector193:

        .weak       vector194
vector194:

        .weak       vector195
vector195:

        .weak       vector196
vector196:

        .weak       vector197
vector197:

        .weak       vector198
vector198:

        .weak       vector199
vector199:

        .weak       vector200
vector200:

        .weak       vector201
vector201:

        .weak       vector202
vector202:

        .weak       vector203
vector203:

        .weak       vector204
vector204:

        .weak       vector205
vector205:

        .weak       vector206
vector206:

        .weak       vector207
vector207:

        .weak       vector208
vector208:

        .weak       vector209
vector209:

        .weak       vector210
vector210:

        .weak       vector211
vector211:

        .weak       vector212
vector212:

        .weak       vector213
vector213:

        .weak       vector214
vector214:

        .weak       vector215
vector215:

        .weak       vector216
vector216:

        .weak       vector217
vector217:

        .weak       vector218
vector218:

        .weak       vector219
vector219:

        .weak       vector220
vector220:

        .weak       vector221
vector221:

        .weak       vector222
vector222:

        .weak       vector223
vector223:

        .weak       vector224
vector224:

        .weak       vector225
vector225:

        .weak       vector226
vector226:

        .weak       vector227
vector227:

        .weak       vector228
vector228:

        .weak       vector229
vector229:

        .weak       vector230
vector230:

        .weak       vector231
vector231:

        .weak       vector232
vector232:

        .weak       vector233
vector233:

        .weak       vector234
vector234:

        .weak       vector235
vector235:

        .weak       vector236
vector236:

        .weak       vector237
vector237:

        .weak       vector238
vector238:

        .weak       vector239
vector239:

        .weak       vector240
vector240:

        .weak       vector241
vector241:

        .weak       vector242
vector242:

        .weak       vector243
vector243:

        .weak       vector244
vector244:

        .weak       vector245
vector245:

        .weak       vector246
vector246:

        .weak       vector247
vector247:

        .weak       vector248
vector248:

        .weak       vector249
vector249:

        .weak       vector250
vector250:

        .weak       vector251
vector251:

        .weak       vector252
vector252:

        .weak       vector253
vector253:

        .weak       vector254
vector254:

        .weak       vector255
vector255:

        .weak       vector256
vector256:

        .weak       vector257
vector257:

        .weak       vector258
vector258:

        .weak       vector259
vector259:

        .weak       vector260
vector260:

        .weak       vector261
vector261:

        .weak       vector262
vector262:

        .weak       vector263
vector263:

        .weak       vector264
vector264:

        .weak       vector265
vector265:

        .weak       vector266
vector266:

        .weak       vector267
vector267:

        .weak       vector268
vector268:

        .weak       vector269
vector269:

        .weak       vector270
vector270:

        .weak       vector271
vector271:

        .weak       vector272
vector272:

        .weak       vector273
vector273:

        .weak       vector274
vector274:

        .weak       vector275
vector275:

        .weak       vector276
vector276:

        .weak       vector277
vector277:

        .weak       vector278
vector278:

        .weak       vector279
vector279:

        .weak       vector280
vector280:

        .weak       vector281
vector281:

        .weak       vector282
vector282:

        .weak       vector283
vector283:

        .weak       vector284
vector284:

        .weak       vector285
vector285:

        .weak       vector286
vector286:

        .weak       vector287
vector287:

        .weak       vector288
vector288:

        .weak       vector289
vector289:

        .weak       vector290
vector290:

        .weak       vector291
vector291:

        .weak       vector292
vector292:

        .weak       vector293
vector293:

        .weak       vector294
vector294:

        .weak       vector295
vector295:

        .weak       vector296
vector296:

        .weak       vector297
vector297:

        .weak       vector298
vector298:

        .weak       vector299
vector299:

        .weak       vector300
vector300:

        .weak       vector301
vector301:

        .weak       vector302
vector302:

        .weak       vector303
vector303:

        .weak       vector304
vector304:

        .weak       vector305
vector305:

        .weak       vector306
vector306:

        .weak       vector307
vector307:

        .weak       vector308
vector308:

        .weak       vector309
vector309:

        .weak       vector310
vector310:

        .weak       vector311
vector311:

        .weak       vector312
vector312:

        .weak       vector313
vector313:

        .weak       vector314
vector314:

        .weak       vector315
vector315:

        .weak       vector316
vector316:

        .weak       vector317
vector317:

        .weak       vector318
vector318:

        .weak       vector319
vector319:

        .weak       vector320
vector320:

        .weak       vector321
vector321:

        .weak       vector322
vector322:

        .weak       vector323
vector323:

        .weak       vector324
vector324:

        .weak       vector325
vector325:

        .weak       vector326
vector326:

        .weak       vector327
vector327:

        .weak       vector328
vector328:

        .weak       vector329
vector329:

        .weak       vector330
vector330:

        .weak       vector331
vector331:

        .weak       vector332
vector332:

        .weak       vector333
vector333:

        .weak       vector334
vector334:

        .weak       vector335
vector335:

        .weak       vector336
vector336:

        .weak       vector337
vector337:

        .weak       vector338
vector338:

        .weak       vector339
vector339:

        .weak       vector340
vector340:

        .weak       vector341
vector341:

        .weak       vector342
vector342:

        .weak       vector343
vector343:

        .weak       vector344
vector344:

        .weak       vector345
vector345:

        .weak       vector346
vector346:

        .weak       vector347
vector347:

        .weak       vector348
vector348:

        .weak       vector349
vector349:

        .weak       vector350
vector350:

        .weak       vector351
vector351:

        .weak       vector352
vector352:

        .weak       vector353
vector353:

        .weak       vector354
vector354:

        .weak       vector355
vector355:

        .weak       vector356
vector356:

        .weak       vector357
vector357:

        .weak       vector358
vector358:

        .weak       vector359
vector359:

        .globl      _unhandled_irq
_unhandled_irq:
         b           _unhandled_irq

/** @endcond */
/** @} */
