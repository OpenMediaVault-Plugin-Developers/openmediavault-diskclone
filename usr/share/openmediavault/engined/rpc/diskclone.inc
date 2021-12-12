<?php
/**
 * Copyright (C) 2021 OpenMediaVault Plugin Developers
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
require_once('openmediavault/functions.inc');

class OMVRpcServiceDiskClone extends \OMV\Rpc\ServiceAbstract
{
    public function getName()
    {
        return 'DiskClone';
    }

    public function initialize()
    {
        $this->registerMethod('doClone');
    }

    public function doClone($params, $context)
    {
        // Validate the RPC caller context.
        $this->validateMethodContext($context, ['role' => OMV_ROLE_ADMINISTRATOR]);

        // Create the background process.
        return $this->execBgProc(function($bgStatusFilename, $bgOutputFilename)
            use ($params) {
                $cmdArgs = [];
                $cmdArgs[] = $params['source'];
                $cmdArgs[] = $params['dest'];
                $cmdArgs[] = $params['method'];
                $cmd = new \OMV\System\Process('/usr/sbin/omv-diskclone', $cmdArgs);
                $cmd->setRedirect2to1(true);
                $cmdLine = $cmd->getCommandLine();
                if (0 !== $this->exec($cmdLine, $output, $bgOutputFilename))
                    throw new \OMV\ExecException($cmdLine, $output);
                return $output;
            }
        );
    }
}
